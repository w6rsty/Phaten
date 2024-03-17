#pragma once

namespace Pt {

enum class ScreenMode
{
    WINDOWED = 0,
    FULLSCREEN,
    BORDERLESS_FULLSCREEN
};

enum class BufferUsage
{
    STATIC = 0,
    DYNAMIC,
};

} // namespace Pt