#include <SDL.h>

#include "Core/Logger.hpp"
#include "Graphics/Graphics.hpp"
#include "Thread/ThreadUtils.hpp"

using namespace Pt;

int main(int argc, char *argv[])
{
    PT_LOG_INFO("Hardware concurrency: ", CPUCount());
    
    auto graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED});

    graphics->SetupCanvas();
    
    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        graphics->RenderCanvas();
        graphics->Present();
    }

    return 0;
}