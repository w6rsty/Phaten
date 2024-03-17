#include "Window.hpp"

#include <SDL.h>
#include <SDL_opengl.h>

#include "Core/Assert.hpp"
#include "Graphics/GraphicsContext.hpp"

namespace Pt
{

Window::Window(std::string_view title, const IntV2& windowSize, ScreenMode mode)
{
    SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitor");
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

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

    IntV2 initialSize = windowSize;

    if (!initialSize.x || !initialSize.y || initialSize.x > displayMode.w || initialSize.y > displayMode.h)
        initialSize = IntV2(displayMode.w, displayMode.h);

    unsigned windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    if (mode == ScreenMode::FULLSCREEN)
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    else if (mode == ScreenMode::BORDERLESS_FULLSCREEN)
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    
    windowHandle = SDL_CreateWindow(
        title.data(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        initialSize.x,
        initialSize.y,
        windowFlags
    );

    if (!windowHandle) {
        PT_LOG_ERROR("Failed to create window");
        return;
    }

    graphicsContext = new GraphicsContext(windowHandle);
    if (!graphicsContext->IsValid())
    {
        PT_LOG_ERROR("Failed to create graphics context");
        return;
    }
}

Window::~Window()
{
    if (windowHandle)
        SDL_DestroyWindow(windowHandle);

    if (graphicsContext)
    {
        delete graphicsContext;
        graphicsContext = nullptr;
    }

    SDL_Quit();
}

void Window::Flush()
{
    graphicsContext->SwapBuffers();
}

} // namespace Pt