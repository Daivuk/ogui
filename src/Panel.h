#pragma once

#include "ogui/IPanel.h"
#include "ogui/types.h"
#include <memory>
#include <vector>

namespace ogui
{
    class IContext;

    class Panel final : public IPanel
    {
    public:
        Panel();
        ~Panel();

        void setTitle(const std::string &title) override;
        const std::string &getTitle() const override { return title; }
        void clear() override;
        void add(const WidgetRef &pWidget) override;
        void insertBefore(const WidgetRef &pWidget, const WidgetRef &pBefore) override;
        void insertAfter(const WidgetRef &pWidget, const WidgetRef &pAfter) override;
        void remove(const WidgetRef &pWidget) override;

        void updateLayout(const Rect &rect);

        IContext *pContext = nullptr;
        Rect tabRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        Rect clientRect = { 0.0f, 0.0f, 0.0f, 0.0f };
        float scrollOffset = 0.0f;
        std::vector<WidgetRef> widgets;
        std::string title = "Panel";
        bool hasCloseButton = false;
    };
}
