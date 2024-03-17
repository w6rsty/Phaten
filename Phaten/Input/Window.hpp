#pragma once

#include "Graphics/GraphicsDefs.hpp"

struct SDL_Window;

namespace Pt {

class Window 
{
public:
    Window();
    ~Window();

    void OnUpdate();

    void Init(ScreenMode mode);
    void Clean();
private:
    bool running {true};
    SDL_Window* window {nullptr};
};

} // namespace Pt