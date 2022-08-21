#include "ogui/Panel.h"
#include "ogui/Control.h"
#include "ogui/IContext.h"

namespace ogui
{
    PanelRef Panel::create()
    {
        return std::shared_ptr<Panel>(new Panel());
    }

    Panel::Panel()
    {
    }

    Panel::~Panel()
    {
    }

    void Panel::setTitle(const std::string &title)
    {
        m_title = title;
    }

    void Panel::clear()
    {
        if (m_controls.empty()) return;

        m_controls.clear();
        
        updateLayout(m_rect);
    }

    void Panel::add(const ControlRef &pControl)
    {
        if (!pControl) return;

        m_controls.push_back(pControl);
        
        updateLayout(m_rect);
    }

    void Panel::insertBefore(const ControlRef &pControl, const ControlRef &pBefore)
    {
        if (!pControl) return;

        auto it = m_controls.begin();
        for (; it != m_controls.end(); ++it)
        {
            if (*it == pBefore)
            {
                break;
            }
        }
        m_controls.insert(it, pControl);
        
        updateLayout(m_rect);
    }

    void Panel::insertAfter(const ControlRef &pControl, const ControlRef &pAfter)
    {
        if (!pControl) return;

        auto it = m_controls.begin();
        for (; it != m_controls.end(); ++it)
        {
            if (*it == pAfter)
            {
                ++it;
                break;
            }
        }
        m_controls.insert(it, pControl);
        
        updateLayout(m_rect);
    }

    void Panel::remove(const ControlRef &pControl)
    {
        if (!pControl) return;

        for (auto it = m_controls.begin(); it != m_controls.end(); ++it)
        {
            if (*it == pControl)
            {
                m_controls.erase(it);
                updateLayout(m_rect);
                break;
            }
        }
    }

    void Panel::updateLayout(const Rect &rect)
    {
        if (m_pContext) m_pContext->setDirty();

        m_rect = rect;
    }
}
