#include <glad/glad.h>

#include <SDL.h>

#include "Input/Window.hpp"

int main(int argc, char *argv[])
{
    Pt::Window window{"Phaten", Pt::IntV2{1000, 600}, Pt::ScreenMode::WINDOWED};

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        window.Flush();
    }

    return 0;
}