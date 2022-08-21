#pragma once

#include "ogui/types.h"
#include <memory>

namespace ogui
{
    class Panel;
    using PanelRef = std::shared_ptr<Panel>;

    class IRenderer;

    /**
    * @brief ogui context object. Application uses this to add/remove Panels, propagate application events and set themes.
    * */
    class IContext
    {
    public:
        /**
        * @brief Creates a new context object. Constructor is private and the Application must call this to create a new Context.
        * 
        * @param pRenderer: Render calls will be done on ogui::IRenderer. Application must inherit from ogui::IRenderer and implement its methods.
        * @param width: Initial width of the application view.
        * @param height: Initial width of the application view.
        * 
        * @note ogui is event driven and will not redraw unless onResize() is called. This is why knowing the initial dimensions state is important.
        * */
        static IContext *create(IRenderer *pRenderer, int width, int height);

        /**
        * @brief Destructor. Unlike creation, Application is free to delete this object as it pleases.
        * */
        virtual ~IContext() {}

        /**
        * @brief Renders the GUIs. Render calls will be called into IRenderer.
        * */
        virtual void render() = 0;

        /**
        * @brief Add a panel to the system. A panel is an Application dockable window. It could be Document View, Properties Panel, Timeline, etc. In ogui, panels have no choice to stay docked and cannot be detached.
        * 
        * @param pPanel: The panel to add.
        * @param pDockParent: Which other panel to dock into. If null, the new panel will be docked in the middle area.
        * @param dockPosition: How to dock in parent. See eDockPosition for details.
        * 
        * @note This will cause a redraw of the screen.
        * */
        virtual void add(const PanelRef &pPanel, const PanelRef &pDockParent = nullptr, eDockPosition dockPosition = eDockPosition::Middle) = 0;

        /**
        * @brief Remove a panel from the system.
        * 
        * @param pPanel: The panel to remove.
        * 
        * @note This will cause a redraw of the screen.
        * */
        virtual void remove(const PanelRef &pPanel) = 0;

        /**
        * @brief Get the current theme.
        * 
        * @return The current theme ogui is using.
        * 
        * @sa Theme
        * */
        virtual const Theme &getTheme() const = 0;

        /**
        * @brief Set a different theme.
        * 
        * @param theme: Application may fill the Theme structure and pass it here.
        * 
        * @sa Theme
        * */
        virtual void setTheme(const Theme &theme) = 0;

        /**
        * @brief Forces the GUI to redraw. This can be used when a control changes visual state and needs to make sure the GUI will be redrawn.
        * */
        virtual void setDirty() = 0;

    public:
        //--------------------------
        //--- Application events ---
        //--------------------------

        /**
        * @brief Application must call this when the Application view changed size.
        * 
        * @param width: The new width of the client area.
        * @param height: The new height of the client area.
        * */
        virtual void onResize(int width, int height) = 0;

        /**
        * @brief Application must call this when the cursor moved inside the client area.
        * 
        * @param x: X coordinate of the cursor in pixels. (0,0) is top-left.
        * @param y: Y coordinate of the cursor in pixels. (0,0) is top-left.
        * */
        virtual void onMouseMove(int x, int y) = 0;

        /**
        * @brief Application must call this when a mouse button is pressed inside the client area.
        * 
        * @param button: Mouse button. 0 = left, 1 = right, 2 = middle, ...
        * */
        virtual void onMouseButtonDown(int button) = 0;

        /**
        * @brief Application must call this when a mouse button is released. With some Windowing API, the event will not trigger when the button is released outside of the client area. It is then recommended to call this when the cursor Leaves the client area.
        * 
        * @param button: Mouse button. 0 = left, 1 = right, 2 = middle, ...
        * */
        virtual void onMouseButtonUp(int button) = 0;

        /**
        * @brief Application must call this when the mouse wheel is scrolled.
        * 
        * @param scroll: Scroll amount. This value can be negative if scroll direction is opposite.
        * */
        virtual void onMouseScroll(int scroll) = 0;

        /**
        * @brief Application must call this when a key is pressed.
        * 
        * @param key: Key code.
        * */
        virtual void onKeyDown(int key) = 0;

        /**
        * @brief Application must call this when a key is released.
        * 
        * @param key: Key code.
        * */
        virtual void onKeyUp(int key) = 0;

        /**
        * @brief Application must call this when text input is entered.
        * 
        * @param text: 
        * 
        * @note As example, in Win32 API, this would be WM_CHAR. In SDL, this would be SDL_TEXTINPUT.
        * */
        virtual void onTextInput(const std::string &text) = 0;

    protected:
        IContext() {}
    };
}
