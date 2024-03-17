#pragma once

#include <string_view>

#include "Graphics/GraphicsDefs.hpp"
#include "Math/IntVector.hpp"

struct SDL_Window;

namespace Pt {

class Window 
{
public:
    Window(std::string_view title, const IntV2& windowSize, ScreenMode mode);
    ~Window();

    void Flush();
private:
    SDL_Window* windowHandle {nullptr};
    class GraphicsContext* graphicsContext;
};

} // namespace Pt