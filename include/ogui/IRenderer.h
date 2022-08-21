#pragma once

#include "ogui/types.h"

namespace ogui
{
    /**
    * Application view must inherit from ogui::IRenderer and implement its methods.
    * */
    class IRenderer
    {
    public:
        /**
        * @brief ogui needs a texture to be created.
        * 
        * @param width: Texture width
        * @param height: Texture height
        * @param pData: Texture data in format RGBA. Total size of pData is width * height * 4.
        * 
        * @return Identifier for the texture. This could be a pointer casted to uintptr_t or a custom identifier used by the Application.
        * 
        * @note This is always called before beginFrame()
        * */
        virtual uintptr_t createTexture(uint32_t width, uint32_t height, uint8_t *pData) = 0;

        /**
        * @brief ogui requests to update the pixels in an existing texture.
        * 
        * @param textureId: Texture identifier that should be recognized by the application.
        * @param width: Texture width
        * @param height: Texture height
        * @param pData: Texture data in format RGBA. Total size of pData is width * height * 4.
        * 
        * @return Identifier for the texture. In most cases, this should be the same identifier passed by this function. Depending on the graphical API used by the Application, a new texture might need to be created and old one deleted.
        * 
        * @note This is always called before beginFrame()
        * */
        virtual uintptr_t updateTexture(uintptr_t textureId, uint32_t width, uint32_t height, uint8_t *pData) = 0;

        /**
        * @brief ogui doesn't need a texture anymore. The Application is free to destroy it.
        * 
        * @param textureId: Texture identifier that should be recognized by the application.
        * 
        * @note This is always called before beginFrame()
        * */
        virtual void destroyTexture(uintptr_t textureId) = 0;

        /**
        * @brief Begin the rendering. All following methods are between beginFrame() and endFrame(). The Application might want to setup render states here. If application want to clear the screen, this is the moment to do it. The Application should not clear the screen otherwise. If the GUI is not dirty, beginFrame() will not be called.
        * */
        virtual void beginFrame() = 0; // Called first

        /**
        * @brief Sets the vertex data for the ogui rendering. This vertex buffer should also be bound to the graphic API. It will be used for subsequent draw calls.
        * 
        * @param pData: Pointer to the first Vertex of the array.
        * @param count: How many vertices there are. Total buffer size is sizeof(Vertex) * count.
        * 
        * @note This is called right after beginFrame() and will not be called multiple times within the same frame.
        * 
        * @sa Vertex
        * */
        virtual void setVertexData(const Vertex *pData, uint32_t count) = 0;

        /**
        * @brief Set graphics scissor rect.
        * 
        * @param x: Offset from the left of the Application's client area.
        * @param y: Offset from the top of the Application's client area.
        * @param width: Width, going right from x.
        * @param height: Height, going down from y.
        * 
        * @note This is assuming (0,0) is top-left. Depending on the graphical API, you might have to invert the Y. For example, in OpenGL:
        * @code{.cpp}
        * glScissor(x, m_height - (y + height), width, height);
        * @endcode
        * */
        virtual void scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        /**
        * @brief Application must bind a texture for the following draw calls.
        * 
        * @param textureId: Texture identifier that should be recognized by the application.
        * */
        virtual void bindTexture(uintptr_t textureId) = 0;

        /**
        * @brief Asks the Applicaiton to draw. The application must render non-indexed triangles.
        * 
        * @param startOffset: Vertex offset from the bound vertex buffer. @see setVertexData
        * @param count: How many vertices to draw.
        * */
        virtual void draw(uint32_t startOffset, uint32_t count) = 0;

        /**
        * @brief Custom draw call inserted by the Application. This can be useful to draw the content of a viewport.
        * 
        * @param userDrawFn: Function pointer the Application must call.
        * @param pUserData: Custom user data set by the Application. The Application may use this to store an object pointer.
        * @param viewport: Current viewport to draw into. [x, y, width, height]
        * 
        * @note The reason ogui doesn't just call userDrawFn directly is so the Application is given the opportunity to reset the render states before and/or after this call.
        * */
        virtual void userDraw(UserDrawFn userDrawFn, void *pUserData, const uint32_t *viewport) = 0;

        /**
        * @brief End of the rendering. The Application might want to present.
        * */
        virtual void endFrame() = 0;
    };
}
