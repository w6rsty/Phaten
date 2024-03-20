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

    /// Call SDL swap window buffer.
    void Flush();
    /// Get SDL native window handle.
    SDL_Window* Handle() const { return m_WindowHandle; }
private:
    /// SDL native window handle.
    SDL_Window* m_WindowHandle {nullptr};
    /// OpenGL context
    ScopedPtr<class GraphicsContext> m_GraphicsContext;
};

} // namespace Pt