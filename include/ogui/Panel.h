#pragma once

#include "types.h"
#include <memory>
#include <string>
#include <vector>

namespace ogui
{
    class Control;
    using ControlRef = std::shared_ptr<Control>;

    class Panel;
    using PanelRef = std::shared_ptr<Panel>;

    class IContext;

    /**
    * @brief A panel is a an Application dockable window. Like Document View, Properties Panel, timeline, etc.
    * */
    class Panel final
    {
    public:
        /**
        * @brief Creates a panel. ogui forces the creation of panels through this function to guarantee that it is created with a shared_ptr. Create a panel doesn't add it to the GUI. The Application may add it to the ogui::IContext.
        * */
        static PanelRef create();

        /**
        * @brief Destructor
        * */
        ~Panel();

        /**
        * @brief Set panel title. This will be visible in the tab bar.
        * 
        * @param title: Title text. Long titles will be cropped.
        * */
        void setTitle(const std::string &title);

        /**
        * @brief Get the panel's title.
        * 
        * @return The panel's current title.
        * */
        const std::string &getTitle() const { return m_title; }

        /**
        * @brief Removes all controls on the panel.
        * */
        void clear();

        /**
        * @brief Adds a control to the panel.
        * 
        * @param pControl: The control to add.
        * */
        void add(const ControlRef &pControl);
        
        /**
        * @brief Inserts a control before another one. Controls in the panel follow a strict draw and alignement order.
        * 
        * @param pControl: The control to insert.
        * @param pBefore: The control already present in the panel which will insert before.
        * */
        void insertBefore(const ControlRef &pControl, const ControlRef &pBefore);

        /**
        * @brief Inserts a control after another one. Controls in the panel follow a strict draw and alignement order.
        * 
        * @param pControl: The control to insert.
        * @param pAfter: The control already present in the panel which will insert after.
        * */
        void insertAfter(const ControlRef &pControl, const ControlRef &pAfter);

        /**
        * @brief Removes a control from the panel.
        * 
        * @param pControl: The control to remove.
        * */
        void remove(const ControlRef &pControl);

    private:
        friend class IContext;

        void updateLayout(const Rect &rect);

        IContext *m_pContext = nullptr;
        
    private:
        Panel();

        Rect m_rect = { 0.0f, 0.0f, 200.0f, 200.0f };
        float m_scrollOffset = 0.0f;
        std::vector<ControlRef> m_controls;
        std::string m_title = "Panel";
    };
}
