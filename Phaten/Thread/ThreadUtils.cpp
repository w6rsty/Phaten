#include "ThreadUtils.hpp"

#include <thread>

namespace Pt {

static std::thread::id mainThreadId = std::this_thread::get_id();

bool IsMainThread()
{
    return mainThreadId == std::this_thread::get_id();
}

unsigned CPUCount()
{
    return std::thread::hardware_concurrency();
}

} // namespace Pt