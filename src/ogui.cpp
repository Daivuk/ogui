#include "ogui/ogui.h"

#include <cassert>

namespace ogui
{
    static uint32_t WHITE = 0xFFFFFFFF;

    enum class eDrawCommand : int32_t
    {
        Draw,
        SetScissor,
        BindTexture,
        UserDraw
    };

    struct DrawCommand
    {
        eDrawCommand command;
        union
        {
            struct DrawData
            {
                uint32_t vertexStart, vertexCount;
            } drawData;

            struct ScissorData
            {
                uint32_t x, y, width, height;
            } scissorData;

            struct BindTextureData
            {
                uintptr_t textureId;
            } bindTextureData;

            struct UserDrawData
            {
                UserDrawFn userDrawFn;
                void *pUserData;
                uint32_t x, y, width, height;
            } userDrawData;
        };
    };

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

    class Context final : public IContext
    {
    public:
        struct Texture
        {
            uint8_t *pData;
            uint32_t width, height;
            uintptr_t id;
        };

        IRenderer *m_pRenderer = nullptr;
        bool m_isDirty = true;
        
        int m_width = 200, m_height = 200;
        int m_mouseX = 0, m_mouseY = 0;

        std::vector<Vertex> m_vertices;
        std::vector<DrawCommand> m_drawList;
        std::vector<Texture *> m_textureToCreate;
        std::vector<Texture *> m_textureToUpdate;
        std::vector<Texture *> m_textureToDestroy;
        std::unordered_map<std::string, Texture *> m_textureByFilename;
        Theme m_theme;

        Texture m_whiteTexture;

        DrawCommand m_drawCmd;
        uintptr_t m_lastBoundTexture = 0;

        Context(IRenderer *pRenderer, int width, int height)
            : m_pRenderer(pRenderer)
            , m_width(width)
            , m_height(height)
        {
            // Setup default theme
            m_theme.panelMargin = 8.0f;
            m_theme.panelPadding = 5.0f;
            m_theme.controlHeight = 26.0f;
            m_theme.headerHeight = 20.0f;
            m_theme.listItemHeight = 20.0f;
            m_theme.borderSize = 1.0f;
            m_theme.tabSpacing = 0.0f;
            m_theme.tabPadding = 8.0f;
            m_theme.toolButtonSize = 16.0f;
            m_theme.toolBarHeight = 20.0f;
            m_theme.treeIndent = 16.0f;
            m_theme.minHSize = 200.0f;
            m_theme.minVSize = 100.0f;
            m_theme.controlMargin = 8.0f;
            m_theme.controlPadding = 4.0f;
            m_theme.controlSpacing = 2.0f;
            m_theme.numericControlWidth = 40.0f;
            m_theme.boolControlWidth = 40.0f;
            m_theme.textControlWidth = 150.0f;

            m_theme.windowColor = HexToColor(0x202531);
            m_theme.darkColor = HexToColor(0x101218);
            m_theme.panelColor = HexToColor(0x333b4f);
            m_theme.panelBorderColor = HexToColor(0x191d27);
            m_theme.areaColor = HexToColor(0x262c3b);
            m_theme.areaBorderColor = HexToColor(0x191d27);
            m_theme.controlColor = HexToColor(0x262c3b);
            m_theme.controlBorderColor = HexToColor(0x202531);
            m_theme.activeColor = HexToColor(0x699ce8);
            m_theme.disabledControlColor = HexToColor(0x2e3547);
            m_theme.disabledControlBorderColor = HexToColor(0x2e3547);
            m_theme.separatorColor = HexToColor(0x333b4f);
            m_theme.textColor = HexToColor(0xe0e0e0);
            m_theme.textOverColor = HexToColor(0xffffff);
            m_theme.disabledTextColor = HexToColor(0x777a82);
            m_theme.toolButtonColor = HexToColor(0xe0e0e0);
            m_theme.toolButtonHover = HexToColor(0xffffff);
            m_theme.toolButtonDown = HexToColor(0x699ce8);
            m_theme.inactiveTabColor = HexToColor(0x262c3b);
            m_theme.dockColor = { m_theme.activeColor.r * 0.5f, m_theme.activeColor.g * 0.5f, m_theme.activeColor.b * 0.5f, m_theme.activeColor.a * 0.5f };
            m_theme.disabledTint = { 0.5f, 0.5f, 0.5f, 1.0f };
            m_theme.headerColor = HexToColor(404553);

            m_whiteTexture.width = 1;
            m_whiteTexture.height = 1;
            m_whiteTexture.pData = (uint8_t *)&WHITE;
            m_textureToCreate.push_back(&m_whiteTexture);

            m_drawCmd.command = eDrawCommand::Draw;
        }

        ~Context() {}

        void render() override
        {
            m_vertices.clear();
            m_drawList.clear();

            m_drawCmd.drawData.vertexStart = 0;
            m_drawCmd.drawData.vertexCount = 0;
            m_lastBoundTexture = 0;

            // Create textures
            for (auto &textureToCreate : m_textureToCreate)
            {
                textureToCreate->id = m_pRenderer->createTexture(textureToCreate->width, textureToCreate->height, textureToCreate->pData);
            }
            m_textureToCreate.clear();

            // Update textures
            for (auto &textureToUpdate : m_textureToUpdate)
            {
                m_pRenderer->updateTexture(textureToUpdate->id, textureToUpdate->width, textureToUpdate->height, textureToUpdate->pData);
            }
            m_textureToUpdate.clear();

            // Destroy textures
            for (auto &textureToDestroy : m_textureToDestroy)
            {
                m_pRenderer->destroyTexture(textureToDestroy->id);
            }
            m_textureToDestroy.clear();

            if (!m_isDirty) return;
            m_isDirty = false;

            // Generate drawlist
            bindTexture(m_whiteTexture);
            drawRect({ 50, 100, 150, 25 }, { 1, 0, 0, 1 });
            flush();

            // Call into the renderer for the actual render
            m_pRenderer->beginFrame();
            m_pRenderer->setVertexData(m_vertices.data(), (uint32_t)m_vertices.size());

            for (const auto &cmd : m_drawList)
            {
                switch (cmd.command)
                {
                    case ogui::eDrawCommand::Draw:
                        m_pRenderer->draw(cmd.drawData.vertexStart, cmd.drawData.vertexCount);
                        break;
                    case ogui::eDrawCommand::SetScissor:
                        m_pRenderer->scissor(cmd.scissorData.x, cmd.scissorData.y, cmd.scissorData.width, cmd.scissorData.height);
                        break;
                    case ogui::eDrawCommand::BindTexture:
                        m_pRenderer->bindTexture(cmd.bindTextureData.textureId);
                        break;
                    case ogui::eDrawCommand::UserDraw:
                        m_pRenderer->userDraw(cmd.userDrawData.userDrawFn, cmd.userDrawData.pUserData, &cmd.userDrawData.x);
                        break;
                };
            }

            m_pRenderer->endFrame();
        }

        const Theme &getTheme() const override { return m_theme; };

        void setTheme(const Theme &theme) override
        {
            m_theme = theme; // TODO: update textures
            m_isDirty = true;
        }

        void onResize(int width, int height) override
        {
            bool shouldUpdateLayout = false;
            if (m_width != width || m_height != height)
                shouldUpdateLayout = true;
            m_width = width;
            m_height = height;
            if (shouldUpdateLayout)
            {
                updateLayout();
            }
        }

        void onMouseMove(int x, int y) override
        {
        }

        void onMouseButtonDown(int button) override
        {
        }

        void onMouseButtonUp(int button) override
        {
        }

        void onMouseScroll(int scroll) override
        {
        }

        void onKeyDown(int key) override
        {
        }

        void onKeyUp(int key) override
        {
        }

        void onTextInput(const std::string &text) override
        {
        }

        void updateLayout()
        {
            m_isDirty = true;
        }

        void flush()
        {
            if (m_drawCmd.drawData.vertexCount)
            {
                m_drawList.push_back(m_drawCmd);
                m_drawCmd.drawData.vertexStart += m_drawCmd.drawData.vertexCount;
                m_drawCmd.drawData.vertexCount = 0;
            }
        }

        void bindTexture(const Texture &texture)
        {
            if (texture.id == m_lastBoundTexture) return;
            flush();

            DrawCommand cmd;
            cmd.command = eDrawCommand::BindTexture;
            cmd.bindTextureData.textureId = texture.id;
            m_drawList.push_back(cmd);

            m_lastBoundTexture = texture.id;
        }

        void drawRect(const Rect &rect, const Color &color)
        {
            auto color32 = ColorToHex(color);

            m_vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });
            m_vertices.push_back(Vertex{ { rect.x, rect.y + rect.h }, { 0, 0 }, color32 });
            m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
            m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
            m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y }, { 0, 0 }, color32 });
            m_vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });

            m_drawCmd.drawData.vertexCount += 6;
        }
    };

    IContext *IContext::create(IRenderer *pRenderer, int width, int height)
    {
        assert(pRenderer && "Must have valid renderer.");
        if (!pRenderer) return nullptr;
        return new Context(pRenderer, width, height);
    }
}
