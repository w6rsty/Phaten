#pragma once

#include "Core/Ptr.hpp"
#include "Input/Window.hpp"
#include "GraphicsContext.hpp"

struct SDL_Window;

namespace Pt {

/// Graphics API interface.
class Graphics
{
public:
    Graphics(WindowCreateInfo windowInfo);

    void Present();
private:
    ScopedPtr<Window> m_Window;
    ScopedPtr<GraphicsContext> m_GraphicsContext;
};

} // namespace Pt