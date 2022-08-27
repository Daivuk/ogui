#pragma once

#include <memory>
#include <string>

namespace ogui
{
    class Widget;
    using WidgetRef = std::shared_ptr<Widget>;

    class IPanel;
    using IPanelRef = std::shared_ptr<IPanel>;

    /**
    * @brief A panel is a an Application dockable window. Like Document View, Properties Panel, timeline, etc.
    * */
    class IPanel
    {
    public:
        /**
        * @brief Creates a panel. ogui forces the creation of panels through this function to guarantee that it is created with a shared_ptr. Create a panel doesn't add it to the GUI. The Application may add it to the ogui::IContext.
        * */
        static IPanelRef create();

        /**
        * @brief Destructor
        * */
        virtual ~IPanel() {}

        /**
        * @brief Set panel title. This will be visible in the tab bar.
        * 
        * @param title: Title text. Long titles will be cropped.
        * */
        virtual void setTitle(const std::string &title) = 0;

        /**
        * @brief Get the panel's title.
        * 
        * @return The panel's current title.
        * */
        virtual const std::string &getTitle() const = 0;

        /**
        * @brief Removes all widgets on the panel.
        * */
        virtual void clear() = 0;

        /**
        * @brief Adds a widget to the panel.
        * 
        * @param pWidget: The widget to add.
        * */
        virtual void add(const WidgetRef &pWidget) = 0;
        
        /**
        * @brief Inserts a widget before another one. Widgets in the panel follow a strict draw and alignement order.
        * 
        * @param pWidget: The widget to insert.
        * @param pBefore: The widget already present in the panel which will insert before.
        * */
        virtual void insertBefore(const WidgetRef &pWidget, const WidgetRef &pBefore) = 0;

        /**
        * @brief Inserts a widget after another one. Widgets in the panel follow a strict draw and alignement order.
        * 
        * @param pWidget: The widget to insert.
        * @param pAfter: The widget already present in the panel which will insert after.
        * */
        virtual void insertAfter(const WidgetRef &pWidget, const WidgetRef &pAfter) = 0;

        /**
        * @brief Removes a widget from the panel.
        * 
        * @param pWidget: The widget to remove.
        * */
        virtual void remove(const WidgetRef &pWidget) = 0;
        
    protected:
        IPanel() {}
    };
}
