#ifndef OGUI_H_INCLUDED
#define OGUI_H_INCLUDED

#include <cinttypes>
#include <string>
#include <unordered_map>
#include <vector>

namespace ogui
{
    // Structures
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

        Color windowColor;
        Color darkColor;
        Color panelColor;
        Color panelBorderColor;
        Color areaColor;
        Color areaBorderColor;
        Color controlColor;
        Color controlBorderColor;
        Color activeColor;
        Color disabledControlColor;
        Color disabledControlBorderColor;
        Color separatorColor;
        Color textColor;
        Color textOverColor;
        Color disabledTextColor;
        Color toolButtonColor;
        Color toolButtonHover;
        Color toolButtonDown;
        Color inactiveTabColor;
        Color dockColor;
        Color disabledTint;
        Color headerColor;
    };
    
    typedef void (*UserDrawFn)(void *pUserData, const uint32_t *pViewport);

    struct DrawCommand;

    class IRenderer
    {
    public:
        // Those happen before beginFrame()
        virtual uintptr_t createTexture(uint32_t width, uint32_t height, uint8_t *pData) = 0;
        virtual void updateTexture(uintptr_t textureId, uint32_t width, uint32_t height, uint8_t *pData) = 0;
        virtual void destroyTexture(uintptr_t textureId) = 0;

        // The draw calls
        virtual void beginFrame() = 0; // Called first
        virtual void setVertexData(const Vertex *pData, uint32_t count) = 0;
        virtual void draw(uint32_t startOffset, uint32_t count) = 0;
        virtual void scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void bindTexture(uintptr_t textureId) = 0;
        virtual void userDraw(UserDrawFn userDrawFn, void *pUserData, const uint32_t *viewport) = 0;
        virtual void endFrame() = 0; // Called last
    };

    class IContext
    {
    public:
        static IContext *create(IRenderer *pRenderer);

        virtual ~IContext() {}

        virtual void update(int width, int height) = 0; // Update GUI and fire events. Pass in the current application width/height
        virtual void render() = 0; // Generate draw list

        virtual const Theme &getTheme() const = 0;
        virtual void setTheme(const Theme &theme) = 0;

    protected:
        IContext() {}
    };
}

#endif /* OGUI_H_INCLUDED */
