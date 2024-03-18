#pragma once

#include <string_view>

#include "Core/Ptr.hpp"
#include "Math/IntVector.hpp"
#include "Graphics/GraphicsDefs.hpp"

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
    ScopedPtr<class GraphicsContext> graphicsContext;
};

} // namespace Pt