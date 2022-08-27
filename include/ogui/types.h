#pragma once

#include <cinttypes>
#include <string>

namespace ogui
{
    struct Color
    {
        float r, g, b, a;
    };

    struct Rect
    {
        float x, y, w, h;
    };

    struct Vec2
    {
        float x, y;
    };

    struct Vertex
    {
        Vec2 position;
        Vec2 uv;
        uint32_t color;
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

    /**
    * @brief User draw callback
    * 
    * @param pUserData: Custom data set by the application through UserDrawControl.
    * @param pViewport: Client area of the control.
    * */
    typedef void (*UserDrawFn)(void *pUserData, const uint32_t *pViewport);

    /**
    * @brief Specifies docking position when adding a new Panel to the Context.
    * */
    enum class eDockPosition
    {
        Center, // Will merge with parent and create a tabbed control.
        Left,   // Split the parent in half horizontally. The inserted panel will be on the left, parent on the right.
        Right,  // Split the parent in half horizontally. The inserted panel will be on the right, parent on the left.
        Top,    // Split the parent in half vertically. The inserted panel will be on the top, parent on the bottom.
        Bottom  // Split the parent in half vertically. The inserted panel will be on the bottom, parent on the top.
    };
}
