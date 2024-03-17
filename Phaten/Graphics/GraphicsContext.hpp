#pragma once

struct SDL_Window;

namespace Pt {

class GraphicsContext
{
public:
    GraphicsContext(SDL_Window* windowHandle);
    ~GraphicsContext();

    bool InitWindowContext();
    bool InitOpenGLContext();
    void Clean();

    void SwapBuffers();

    bool IsValid() const { return isValid; }
private:
    SDL_Window* windowHandle {nullptr};
    void* contextHandle {nullptr};
    bool isValid {false};
};

} // namespaec Pt