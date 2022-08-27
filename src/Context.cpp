#include "ogui/IContext.h"
#include "Context.h"
#include "ogui/IRenderer.h"
#include "Panel.h"
#include "PanelsManager.h"

#include <cassert>
#include <vector>

namespace ogui
{
    static uint32_t WHITE = 0xFFFFFFFF;

    static Color HexToColor(uint32_t hex)
    {
        return {
            static_cast<float>((hex & 0xff0000) >> 16) / 255.f,
            static_cast<float>((hex & 0x00ff00) >> 8) / 255.f,
            static_cast<float>((hex & 0x0000ff)) / 255.f,
            1.f
        };
    }

    static uint32_t ColorToHex(Color col)
    {
        uint32_t packed = 0;
        packed |= ((uint32_t)(col.r * 255.f) << 24) & 0xff000000;
        packed |= ((uint32_t)(col.g * 255.f) << 16) & 0x00ff0000;
        packed |= ((uint32_t)(col.b * 255.f) << 8) & 0x0000ff00;
        packed |= (uint32_t)(col.a * 255.f) & 0x000000ff;
        return packed;
    }

    Context::Context(IRenderer *in_pRenderer, int in_width, int in_height)
        : pRenderer(in_pRenderer)
        , width(in_width)
        , height(in_height)
    {
        // Setup default theme
        theme.panelMargin = 8.0f;
        theme.panelPadding = 5.0f;
        theme.controlHeight = 26.0f;
        theme.headerHeight = 20.0f;
        theme.listItemHeight = 20.0f;
        theme.borderSize = 1.0f;
        theme.tabSpacing = 0.0f;
        theme.tabPadding = 8.0f;
        theme.toolButtonSize = 16.0f;
        theme.toolBarHeight = 20.0f;
        theme.treeIndent = 16.0f;
        theme.minHSize = 200.0f;
        theme.minVSize = 100.0f;
        theme.controlMargin = 8.0f;
        theme.controlPadding = 4.0f;
        theme.controlSpacing = 2.0f;
        theme.numericControlWidth = 40.0f;
        theme.boolControlWidth = 40.0f;
        theme.textControlWidth = 150.0f;

        theme.windowColor = HexToColor(0x202531);
        theme.darkColor = HexToColor(0x101218);
        theme.panelColor = HexToColor(0x333b4f);
        theme.panelBorderColor = HexToColor(0x191d27);
        theme.areaColor = HexToColor(0x262c3b);
        theme.areaBorderColor = HexToColor(0x191d27);
        theme.controlColor = HexToColor(0x262c3b);
        theme.controlBorderColor = HexToColor(0x202531);
        theme.activeColor = HexToColor(0x699ce8);
        theme.disabledControlColor = HexToColor(0x2e3547);
        theme.disabledControlBorderColor = HexToColor(0x2e3547);
        theme.separatorColor = HexToColor(0x333b4f);
        theme.textColor = HexToColor(0xe0e0e0);
        theme.textOverColor = HexToColor(0xffffff);
        theme.disabledTextColor = HexToColor(0x777a82);
        theme.toolButtonColor = HexToColor(0xe0e0e0);
        theme.toolButtonHover = HexToColor(0xffffff);
        theme.toolButtonDown = HexToColor(0x699ce8);
        theme.inactiveTabColor = HexToColor(0x262c3b);
        theme.dockColor = { theme.activeColor.r * 0.5f, theme.activeColor.g * 0.5f, theme.activeColor.b * 0.5f, theme.activeColor.a * 0.5f };
        theme.disabledTint = { 0.5f, 0.5f, 0.5f, 1.0f };
        theme.headerColor = HexToColor(404553);

        whiteTexture.width = 1;
        whiteTexture.height = 1;
        whiteTexture.pData = (uint8_t *)&WHITE;
        textureToCreate.push_back(&whiteTexture);

        // Re-usable draw command.
        drawCmd.command = eDrawCommand::Draw;

        pPanelsManager = new PanelsManager();
    }

    Context::~Context()
    {
        delete pPanelsManager;
    }

    void Context::add(const IPanelRef &pPanel, const IPanelRef &pDockParent, eDockPosition dockPosition)
    {
        auto pPanelImpl = std::dynamic_pointer_cast<Panel>(pPanel);
        auto pDockParentImpl = std::dynamic_pointer_cast<Panel>(pDockParent);

        if (!pPanelImpl) return;

        for (const auto &pOwnPanel : panels) if (pOwnPanel == pPanel) return; // Aleady added

        if (!pDockParentImpl)
        {
            pPanelsManager->document_zone->panels.push_back(pPanelImpl);
            pPanelsManager->document_zone->active_panel = (int)pPanelsManager->document_zone->panels.size() - 1;
        }
        else
        {
            int index;
            auto pDockZone = pPanelsManager->find(pDockParentImpl, &index);
            if (pDockZone == nullptr) pDockZone = pPanelsManager->document_zone;

            if (dockPosition == eDockPosition::Center)
            {
                pDockZone->panels.push_back(pPanelImpl);
                pDockZone->active_panel = (int)pDockZone->panels.size() - 1;
            }
            else
            {
                DockContext dockContext;
                dockContext.target = pDockZone;
                dockContext.position = (eDockPanelPosition)dockPosition;
                dockContext.tab_index = 0;
                dockContext.amount = 0.5f;
                dockContext.magnet = eDockMagnet::Middle;
                dockContext.left_most = true;
                dockContext.right_most = true;
                dockContext.top_most = true;
                dockContext.bottom_most = true;
                pDockZone->dockPanel(pPanelImpl, dockContext);
            }
        }

        updateLayout();
    }

    void Context::remove(const IPanelRef &pPanel)
    {
        auto pPanelImpl = std::dynamic_pointer_cast<Panel>(pPanel);

        if (!pPanelImpl) return;
        for (auto it = panels.begin(); it != panels.end(); ++it)
        {
            if (*it == pPanelImpl)
            {
                panels.erase(it);
                updateLayout();
                break;
            }
        }
    }

    void Context::render()
    {
        vertices.clear();
        drawList.clear();

        drawCmd.drawData.vertexStart = 0;
        drawCmd.drawData.vertexCount = 0;
        lastBoundTexture = 0;

        // Create textures
        for (auto &textureToCreate : textureToCreate)
        {
            textureToCreate->id = pRenderer->createTexture(textureToCreate->width, textureToCreate->height, textureToCreate->pData);
        }
        textureToCreate.clear();

        // Update textures
        for (auto &textureToUpdate : textureToUpdate)
        {
            pRenderer->updateTexture(textureToUpdate->id, textureToUpdate->width, textureToUpdate->height, textureToUpdate->pData);
        }
        textureToUpdate.clear();

        // Destroy textures
        for (auto &textureToDestroy : textureToDestroy)
        {
            pRenderer->destroyTexture(textureToDestroy->id);
        }
        textureToDestroy.clear();

        if (!isDirty) return;
        isDirty = false;

        // Generate drawlist
        bindTexture(whiteTexture);
        drawRect({ 50, 100, 150, 25 }, { 1, 0, 0, 1 });
        flush();

        // Draw panels
        pPanelsManager->render(this);

        // Call into the renderer for the actual render
        pRenderer->beginFrame();
        pRenderer->setVertexData(vertices.data(), (uint32_t)vertices.size());

        for (const auto &cmd : drawList)
        {
            switch (cmd.command)
            {
                case ogui::eDrawCommand::Draw:
                    pRenderer->draw(cmd.drawData.vertexStart, cmd.drawData.vertexCount);
                    break;
                case ogui::eDrawCommand::SetScissor:
                    pRenderer->scissor(cmd.scissorData.x, cmd.scissorData.y, cmd.scissorData.width, cmd.scissorData.height);
                    break;
                case ogui::eDrawCommand::BindTexture:
                    pRenderer->bindTexture(cmd.bindTextureData.textureId);
                    break;
                case ogui::eDrawCommand::UserDraw:
                    pRenderer->userDraw(cmd.userDrawData.userDrawFn, cmd.userDrawData.pUserData, &cmd.userDrawData.x);
                    break;
            };
        }

        pRenderer->endFrame();
    }

    void Context::setTheme(const Theme &in_theme)
    {
        theme = in_theme; // TODO: update textures
        isDirty = true;
    }

    void Context::setDirty()
    {
        isDirty = true;
    }

    void Context::onResize(int in_width, int in_height)
    {
        bool shouldUpdateLayout = false;
        if (width != in_width || height != in_height)
            shouldUpdateLayout = true;
        width = in_width;
        height = in_height;
        if (shouldUpdateLayout)
        {
            updateLayout();
        }
    }

    void Context::onMouseMove(int x, int y)
    {
    }

    void Context::onMouseButtonDown(int button)
    {
    }

    void Context::onMouseButtonUp(int button)
    {
    }

    void Context::onMouseScroll(int scroll)
    {
    }

    void Context::onKeyDown(int key)
    {
    }

    void Context::onKeyUp(int key)
    {
    }

    void Context::onTextInput(const std::string &text)
    {
    }

    void Context::updateLayout()
    {
        pPanelsManager->updateLayout(this);
        isDirty = true;
    }

    void Context::flush()
    {
        if (drawCmd.drawData.vertexCount)
        {
            drawList.push_back(drawCmd);
            drawCmd.drawData.vertexStart += drawCmd.drawData.vertexCount;
            drawCmd.drawData.vertexCount = 0;
        }
    }

    void Context::bindTexture(const Texture &texture)
    {
        if (texture.id == lastBoundTexture) return;
        flush();

        DrawCommand cmd;
        cmd.command = eDrawCommand::BindTexture;
        cmd.bindTextureData.textureId = texture.id;
        drawList.push_back(cmd);

        lastBoundTexture = texture.id;
    }

    void Context::drawRect(const Rect &rect, const Color &color)
    {
        auto color32 = ColorToHex(color);

        vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });
        vertices.push_back(Vertex{ { rect.x, rect.y + rect.h }, { 0, 0 }, color32 });
        vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
        vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
        vertices.push_back(Vertex{ { rect.x + rect.w, rect.y }, { 0, 0 }, color32 });
        vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });

        drawCmd.drawData.vertexCount += 6;
    }

    IContext *IContext::create(IRenderer *pRenderer, int width, int height)
    {
        assert(pRenderer && "Must have valid renderer.");
        if (!pRenderer) return nullptr;
        return new Context(pRenderer, width, height);
    }
}
