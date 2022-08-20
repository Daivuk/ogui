#ifndef OGUI_H_INCLUDED
#define OGUI_H_INCLUDED

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <vector>

namespace ogui
{
    enum class eDrawCommand : int32_t
    {
        Draw,
        SetScissor,
        SetTexture,
        CreateTexture,
        UpdateTexture,
        DestroyTexture,
        UserDraw
    };

    struct Vec2
    {
        float x, y;
    };

    struct Rect
    {
        float x, y, w, h;
    };

    struct Vertex
    {
        Vec2 position;
        Vec2 uv;
        uint32_t color;
    };

    typedef void (*UserDrawFn)(void *pUserData, const uint32_t *pViewport);

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

            struct SetTextureData
            {
                uintptr_t textureId;
            } setTextureData;

            struct CreateTextureData
            {
                uintptr_t *pTextureId;
                uint32_t width, height;
                uint8_t *pData;
            } createTextureData;

            struct UpdateTextureData
            {
                uintptr_t textureId;
                uint32_t width, height;
                uint8_t *pData;
            } updateTextureData;

            struct DestroyTextureData
            {
                uintptr_t textureId;
            } destroyTextureData;

            struct UserDrawData
            {
                UserDrawFn userDrawFn;
                void *pUserData;
                uint32_t x, y, width, height;
            } userDrawData;
        };
    };
    
    struct Color
    {
        float r, g, b, a;
    };

    struct Theme
    {
        std::string font;
        float fontSize;

        std::string newIcon;
        std::string openIcon;
        std::string saveIcon;
        std::string undoIcon;
        std::string redoIcon;
        std::string selectIcon;
        std::string moveIcon;
        std::string rotateIcon;
        std::string scaleIcon;

        std::string scrollbar;
        std::string xIcon;

        float panelMargin;
        float panelPadding;
        float controlHeight;
        float headerHeight;
        float listItemHeight;
        float borderSize;
        float tabSpacing;
        float tabPadding;
        float toolButtonSize;
        float toolBarHeight;
        float treeIndent;
        float minHSize;
        float minVSize;
        float controlMargin;
        float controlPadding;
        float controlSpacing;
        float numericControlWidth;
        float boolControlWidth;
        float textControlWidth;

        Color windowColor;;
        Color darkColor;;
        Color panelColor;;
        Color panelBorderColor;;
        Color areaColor;;
        Color areaBorderColor;;
        Color controlColor;;
        Color controlBorderColor;;
        Color activeColor;;
        Color disabledControlColor;;
        Color disabledControlBorderColor;;
        Color separatorColor;;
        Color textColor;;
        Color textOverColor;;
        Color disabledTextColor;;
        Color toolButtonColor;;
        Color toolButtonHover;;
        Color toolButtonDown;;
        Color inactiveTabColor;;
        Color dockColor;;
        Color disabledTint;;
        Color headerColor;;
    };

    class Context final
    {
    public:
        Context();
        ~Context();

        void update(int width, int height); // Update GUI and fire events. Pass in the current application width/height
        void render(); // Generate draw list

        const std::vector<Vertex> &getVertices() const { return m_vertices; }
        const std::vector<DrawCommand> &getDrawList() const { return m_drawList; }

        const Theme &getTheme() const { return m_theme; };
        void setTheme(const Theme &theme);

    private:
        struct Texture
        {
            uint8_t *pData;
            uint32_t width, height;
            uintptr_t id;
        };

        void bindTexture(const Texture &texture);
        void drawRect(const Rect &rect, const Color &color);

        std::vector<Vertex> m_vertices;
        std::vector<DrawCommand> m_drawList;
        std::vector<Texture *> m_textureToCreate;
        std::vector<Texture *> m_textureToUpdate;
        std::vector<Texture *> m_textureToDestroy;
        std::unordered_map<std::string, Texture *> m_textureByFilename;
        Theme m_theme;

        Texture m_whiteTexture;
    };
}

#endif /* OGUI_H_INCLUDED */
