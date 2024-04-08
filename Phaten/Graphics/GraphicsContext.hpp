#pragma once

#include "Object/Ptr.hpp"
struct SDL_Window;

namespace Pt {

/// Consists of a window handle and an OpenGL context handle.
class GraphicsContext
{
    friend class Graphics;
public:
    GraphicsContext(SDL_Window* windowHandle);
    virtual ~GraphicsContext();

    bool IsValid() const { return m_IsValid; }
private:
    bool InitWindowContext();
    bool InitOpenGLContext();
    void Release();

    SDL_Window* m_WindowHandle;
    void* m_GLContextHandle;
    bool m_IsValid;
};

} // namespace Pt