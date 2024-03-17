#include "Window.hpp"
#include "SDL_video.h"

#include <chrono>
#include <thread>

#include <SDL.h>
#include <SDL_opengl.h>

namespace Pt
{

Window::Window()
{
    Init(ScreenMode::WINDOWED);
}

Window::~Window()
{
    Clean();
}

void Window::Init(ScreenMode mode)
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

    unsigned windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;
    if (mode == ScreenMode::FULLSCREEN)
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    else if (mode == ScreenMode::BORDERLESS_FULLSCREEN)
        windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    
    window = SDL_CreateWindow(
        "Phaten",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        windowFlags);
}

void Window::Clean()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::OnUpdate()
{
    SDL_Event event;
    bool bQuit {false};

    while (!bQuit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
             bQuit = true;

            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.type == SDL_WINDOWEVENT_MINIMIZED)
                    running = false;
                if (event.window.event == SDL_WINDOWEVENT_RESTORED)
                    running = true;
            }
        }

        if (!running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        // render
    }
}

} // namespace Pt