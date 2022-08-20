#include "ogui/ogui.h"

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

    Context::Context()
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
    }

    Context::~Context()
    {
    }

    void Context::setTheme(const Theme &theme)
    {
    }

    void Context::update(int width, int height)
    {
    }

    void Context::render()
    {
        m_vertices.clear();
        m_drawList.clear();

        // Create textures
        for (auto &textureToCreate : m_textureToCreate)
        {
            DrawCommand cmd;
            cmd.command = eDrawCommand::CreateTexture;
            cmd.createTextureData.pTextureId = &textureToCreate->id;
            cmd.createTextureData.width = textureToCreate->width;
            cmd.createTextureData.height = textureToCreate->height;
            cmd.createTextureData.pData = textureToCreate->pData;
            m_drawList.push_back(cmd);
        }
        m_textureToCreate.clear();

        // Update textures
        for (auto &textureToUpdate : m_textureToUpdate)
        {
            DrawCommand cmd;
            cmd.command = eDrawCommand::UpdateTexture;
            cmd.updateTextureData.textureId = textureToUpdate->id;
            cmd.updateTextureData.width = textureToUpdate->width;
            cmd.updateTextureData.height = textureToUpdate->height;
            cmd.updateTextureData.pData = textureToUpdate->pData;
            m_drawList.push_back(cmd);
        }
        m_textureToUpdate.clear();

        // Destroy textures
        for (auto &textureToDestroy : m_textureToDestroy)
        {
            DrawCommand cmd;
            cmd.command = eDrawCommand::UpdateTexture;
            cmd.destroyTextureData.textureId = textureToDestroy->id;
            m_drawList.push_back(cmd);
        }
        m_textureToDestroy.clear();

        // Draw a simple quad to test
        bindTexture(m_whiteTexture);
        drawRect({ 50, 100, 150, 25 }, { 1, 0, 0, 1 });
    }

    void Context::bindTexture(const Texture &texture)
    {
        DrawCommand cmd;
        cmd.command = eDrawCommand::SetTexture;
        cmd.setTextureData.textureId = m_whiteTexture.id;
        m_drawList.push_back(cmd);
    }

    void Context::drawRect(const Rect &rect, const Color &color)
    {
        auto color32 = ColorToHex(color);

        m_vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });
        m_vertices.push_back(Vertex{ { rect.x, rect.y + rect.h }, { 0, 0 }, color32 });
        m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
        m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y + rect.h }, { 0, 0 }, color32 });
        m_vertices.push_back(Vertex{ { rect.x + rect.w, rect.y }, { 0, 0 }, color32 });
        m_vertices.push_back(Vertex{ { rect.x, rect.y }, { 0, 0 }, color32 });

        DrawCommand cmd;
        cmd.command = eDrawCommand::Draw;
        cmd.drawData.vertexStart = 0;
        cmd.drawData.vertexCount = 6;
        m_drawList.push_back(cmd);
    }
}
