#include <SDL.h>

#include "IO/Logger.hpp"
#include "Graphics/Graphics.hpp"
#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"
#include "Thread/ThreadUtils.hpp"

#include "Math/Transform.hpp"

using namespace Pt;

int main(int argc, char *argv[])
{
    // PT_LOG_INFO("Hardware concurrency: ", CPUCount());
    
    // auto graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED});

    // graphics->SetupCanvas();
    
    // bool running = true;
    // SDL_Event event;
    // while (running)
    // {
    //     while (SDL_PollEvent(&event))
    //     {
    //         if (event.type == SDL_QUIT)
    //         {
    //             running = false;
    //         }
    //     }

    //     graphics->RenderCanvas();
    //     graphics->Present();
    // }

    // test invserse
    Matrix4 m{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    return 0;
}