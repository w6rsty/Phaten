#pragma once

#include <string_view>

#include "Object/Ptr.hpp"
#include "Math/IntVector.hpp"
#include "Graphics/GraphicsDefs.hpp"

struct SDL_Window;

namespace Pt {

struct WindowCreateInfo
{
    std::string_view title;
    IntV2 windowSize;
    ScreenMode mode;
};

class Window 
{
public:
    Window(std::string_view title, const IntV2& windowSize, ScreenMode mode);
    ~Window();

    /// Get SDL native window handle.
    SDL_Window* NativeHandle() const { return m_WindowHandle; }
private:
    /// SDL native window handle.
    SDL_Window* m_WindowHandle {nullptr};
};

} // namespace Pt