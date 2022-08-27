#pragma once

#include "ogui/IContext.h"
#include <vector>

namespace ogui
{
    enum class eDrawCommand : int32_t
    {
        Draw,
        SetScissor,
        BindTexture,
        UserDraw
    };

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

            struct BindTextureData
            {
                uintptr_t textureId;
            } bindTextureData;

            struct UserDrawData
            {
                UserDrawFn userDrawFn;
                void *pUserData;
                uint32_t x, y, width, height;
            } userDrawData;
        };
    };

    class PanelsManager;

    class Panel;
    using PanelRef = std::shared_ptr<Panel>;

    class Context final : public IContext
    {
    public:
        struct Texture
        {
            uint8_t *pData;
            uint32_t width, height;
            uintptr_t id;
        };

        Context(IRenderer *pRenderer, int width, int height);
        ~Context();

        void add(const IPanelRef &pPanel, const IPanelRef &pDockParent, eDockPosition dockPosition) override;
        void remove(const IPanelRef &pPanel) override;

        void render() override;

        const Theme &getTheme() const override { return theme; };
        void setTheme(const Theme &theme) override;

        void setDirty() override;

        void onResize(int width, int height) override;
        void onMouseMove(int x, int y) override;
        void onMouseButtonDown(int button) override;
        void onMouseButtonUp(int button) override;
        void onMouseScroll(int scroll) override;
        void onKeyDown(int key) override;
        void onKeyUp(int key) override;
        void onTextInput(const std::string &text) override;

        void updateLayout();

        void flush();
        void bindTexture(const Texture &texture);
        void drawRect(const Rect &rect, const Color &color);

        Rect getRect() const { return { 0.0f, 0.0f, (float)width, (float)height }; }

    public:
        IRenderer *pRenderer = nullptr;
        bool isDirty = true;
        
        int width = 200, height = 200;
        int mouseX = 0, mouseY = 0;

        std::vector<Vertex> vertices;
        std::vector<DrawCommand> drawList;
        std::vector<Texture *> textureToCreate;
        std::vector<Texture *> textureToUpdate;
        std::vector<Texture *> textureToDestroy;
        Theme theme;

        Texture whiteTexture;

        DrawCommand drawCmd;
        uintptr_t lastBoundTexture = 0;

        PanelsManager *pPanelsManager = nullptr;
        std::vector<PanelRef> panels;
    };
}
