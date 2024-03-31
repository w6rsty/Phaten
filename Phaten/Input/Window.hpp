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

class Window : public RefCounted
{
public:
    Window(WindowCreateInfo info);
    ~Window();

    void Swap();

    void SetVSync(bool enable);
    IntV2 Size() const;
    unsigned Time() const;
    /// Get SDL native window handle.
    SDL_Window* SDLHandle() const { return m_Handle; }
private:
    /// SDL native window handle.
    SDL_Window* m_Handle {nullptr};
};

} // namespace Pt