# ogui
Event based GUI for tools and editors.

## Main Loop
```cpp
bool done = false;
while (!done)
{
    m_pGuiContext->render();

    SDL_Event event;
    if (!SDL_WaitEvent(&event))
    {
        continue; // Error
    }

    switch (event.type)
    {
        case SDL_QUIT:
            break;
        case SDL_WINDOWEVENT:
            if (event.window.windowID == SDL_GetWindowID(m_pWindow))
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        done = true;
                        break;
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        m_pGuiContext->onResize(event.window.data1, event.window.data2);
                        break;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            m_pGuiContext->onMouseMove(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_pGuiContext->onMouseButtonDown(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            m_pGuiContext->onMouseButtonUp(event.button.button);
            break;
        case SDL_MOUSEWHEEL:
            m_pGuiContext->onMouseScroll(event.wheel.y);
            break;
        case SDL_KEYDOWN:
            m_pGuiContext->onKeyDown(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            m_pGuiContext->onKeyUp(event.key.keysym.sym);
            break;
        case SDL_TEXTINPUT:
            m_pGuiContext->onTextInput(event.text.text);
            break;
    }
}
```
