#pragma once

#include "ogui/types.h"
#include <memory>
#include <vector>

namespace ogui
{
    enum class eDockMagnet
    {
        Left    = 0,
        Top     = 0,
        Middle  = 1,
        Right   = 2,
        Bottom  = 2
    };

    enum class eDockPanelPosition
    {
        Center,
        Left,
        Right,
        Top,
        Bottom,
        Tab
    };

    struct DockContext;

    class DockNode;
    using DockNodeRef = std::shared_ptr<DockNode>;

    class DockZone;
    using DockZoneRef = std::shared_ptr<DockZone>;

    class Context;

    class Panel;
    using PanelRef = std::shared_ptr<Panel>;
    
    class DockNode
    {
    public:
        virtual void render(Context* ctx) = 0;
        virtual void updateLayout(const Rect &parentRect, Context* ctx) = 0;
        virtual void dock(Context* ctx, DockContext* dock_ctx) = 0;
        virtual void undockPanel(const PanelRef& panel) = 0;
        virtual DockNodeRef clean() = 0;
        virtual DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) = 0;
        virtual DockZoneRef find(const PanelRef& panel, int* index) = 0;
    };

    class DockNull final : public DockNode
    {
    public:
        void render(Context* ctx) override {};
        void updateLayout(const Rect &parentRect, Context* ctx) override {};
        void dock(Context* ctx, DockContext* dock_ctx) override {};
        void undockPanel(const PanelRef& panel) override {};
        DockNodeRef clean() override { return nullptr; };
        DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override { return nullptr; };
        DockZoneRef find(const PanelRef& panel, int* index) override { return nullptr; }
    };

    // Dock zone is a leaf node. Containing one or many panels with tabs
    class DockZone : public DockNode, public std::enable_shared_from_this<DockZone>
    {
    public:
        std::vector<PanelRef>   panels;
        int                     active_panel = 0;

        DockZone(const std::vector<PanelRef>& panels, int active_panel);

        void render(Context* ctx) override;
        void updateLayout(const Rect &parentRect, Context* ctx) override;
        void dock(Context* ctx, DockContext* dock_ctx) override;
        void undockPanel(const PanelRef& panel) override;
        DockNodeRef clean() override;
        DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
        DockZoneRef find(const PanelRef& panel, int* index) override;
    };

    class DockKeepAround final : public DockZone
    {
    public:
        std::string text;

        DockKeepAround(const std::string& text, const std::vector<PanelRef>& panels, int active_panel);

        void render(Context* ctx) override;
        DockNodeRef clean() override;
    };

    class DockHSplit final : public DockNode, public std::enable_shared_from_this<DockHSplit>
    {
    public:
        DockNodeRef left;
        DockNodeRef right;
        float       amount = 300.0f;
        eDockMagnet magnet = eDockMagnet::Left;

        DockHSplit(DockNodeRef left, DockNodeRef right, float amount, eDockMagnet magnet);

        void render(Context* ctx) override;
        void updateLayout(const Rect &parentRect, Context* ctx) override;
        void dock(Context* ctx, DockContext* dock_ctx) override;
        void undockPanel(const PanelRef& panel) override;
        DockNodeRef clean() override;
        DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
        DockZoneRef find(const PanelRef& panel, int* index) override;
    };

    class DockVSplit final : public DockNode, public std::enable_shared_from_this<DockVSplit>
    {
    public:
        DockNodeRef top;
        DockNodeRef bottom;
        float       amount = 200.0f;
        eDockMagnet magnet = eDockMagnet::Bottom;

        DockVSplit(DockNodeRef top, DockNodeRef bottom, float amount, eDockMagnet magnet);

        void render(Context* ctx) override;
        void updateLayout(const Rect &parentRect, Context* ctx) override;
        void dock(Context* ctx, DockContext* result) override;
        void undockPanel(const PanelRef& panel) override;
        DockNodeRef clean() override;
        DockNodeRef dockPanel(const PanelRef& panel, const DockContext& dock_ctx) override;
        DockZoneRef find(const PanelRef& panel, int* index) override;
    };

    struct DockContext
    {
        DockZoneRef         target;
        eDockPanelPosition  position    = eDockPanelPosition::Center;
        int                 tab_index   = 0;
        float               amount      = 0.5f;
        eDockMagnet         magnet      = eDockMagnet::Middle;
        bool                left_most   = true;
        bool                right_most  = true;
        bool                top_most    = true;
        bool                bottom_most = true;
    };

    class PanelsManager final
    {
    public:
        DockNodeRef             dock_root;
        PanelRef                dragging_panel;
        DockNodeRef             dragging_split;
        DockZoneRef             document_zone;
        Rect                    dragging_split_rect;
        bool                    dropped_panel   = false;
        bool                    dropped_split   = false;
        bool                    closed_panel    = false;

        PanelsManager();
        ~PanelsManager();

        void undockPanel(const PanelRef& panel);
        void dockPanel(const PanelRef& panel, const DockContext& dock_ctx); 
        void cleanDock();
        DockZoneRef find(const PanelRef& panel, int* index);
        
        void updateLayout(Context* ctx);
        void render(Context* ctx);
    };
}
