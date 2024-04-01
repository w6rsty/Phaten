#include "Window.hpp"

#include <SDL.h>
#include <SDL_opengl.h>

#include "IO/Assert.hpp"
#include "Math/IntVector.hpp"

namespace Pt
{

Window::Window(WindowCreateInfo info)
{
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 

    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);

    IntV2 initialSize = info.windowSize;

    if (!initialSize.x ||  initialSize.x > displayMode.w)
    {
        initialSize.x = displayMode.w;
    }
    if (!initialSize.y || initialSize.y > displayMode.h)
    {
        initialSize.y = displayMode.h;
    }

    unsigned windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    if (info.mode == ScreenMode::FULLSCREEN)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    }
    else if (info.mode == ScreenMode::BORDERLESS_FULLSCREEN)
    {
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    
    m_Handle = SDL_CreateWindow(
        info.title.data(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        initialSize.x,
        initialSize.y,
        windowFlags
    );

    if (!m_Handle) {
        PT_LOG_ERROR("Failed to create window");
        return;
    }

    PT_LOG_INFO("Created Window: ", info.title, " ", initialSize.x, "x", initialSize.y);
}

Window::~Window()
{
    if (m_Handle)
    {
        SDL_DestroyWindow(SDL_GL_GetCurrentWindow());
    }

    SDL_Quit();
}

void Window::SetVSync(bool enable)
{
    SDL_GL_SetSwapInterval(enable ? 1 : 0);
}

IntV2 Window::Size() const
{
    IntV2 size;
    SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), (int*)&size.x, (int*)&size.y);
    return size;
}

unsigned Window::Time() const
{
    return SDL_GetTicks();
}

void Window::Swap()
{
    SDL_GL_SwapWindow(SDL_GL_GetCurrentWindow());
}


} // namespace Pt