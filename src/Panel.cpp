#include "Panel.h"
#include "ogui/IContext.h"
#include "ogui/Widget.h"

namespace ogui
{
    IPanelRef IPanel::create()
    {
        return std::shared_ptr<Panel>(new Panel());
    }

    Panel::Panel()
    {
    }

    Panel::~Panel()
    {
    }

    void Panel::setTitle(const std::string &in_title)
    {
        title = in_title;
    }

    void Panel::clear()
    {
        if (widgets.empty()) return;

        widgets.clear();
        
        updateLayout(clientRect);
    }

    void Panel::add(const WidgetRef &pWidget)
    {
        if (!pWidget) return;

        widgets.push_back(pWidget);
        
        updateLayout(clientRect);
    }

    void Panel::insertBefore(const WidgetRef &pWidget, const WidgetRef &pBefore)
    {
        if (!pWidget) return;

        auto it = widgets.begin();
        for (; it != widgets.end(); ++it)
        {
            if (*it == pBefore)
            {
                break;
            }
        }
        widgets.insert(it, pWidget);
        
        updateLayout(clientRect);
    }

    void Panel::insertAfter(const WidgetRef &pWidget, const WidgetRef &pAfter)
    {
        if (!pWidget) return;

        auto it = widgets.begin();
        for (; it != widgets.end(); ++it)
        {
            if (*it == pAfter)
            {
                ++it;
                break;
            }
        }
        widgets.insert(it, pWidget);
        
        updateLayout(clientRect);
    }

    void Panel::remove(const WidgetRef &pWidget)
    {
        if (!pWidget) return;

        for (auto it = widgets.begin(); it != widgets.end(); ++it)
        {
            if (*it == pWidget)
            {
                widgets.erase(it);
                updateLayout(clientRect);
                break;
            }
        }
    }

    void Panel::updateLayout(const Rect &rect)
    {
        if (pContext) pContext->setDirty();

        clientRect = rect;
    }
}
