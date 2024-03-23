#include "StringHash.hpp"

#include <cctype>

#include "StringUtils.hpp"

namespace Pt {

const StringHash StringHash::ZERO;

std::string StringHash::ToString() const
{
    return FormatString("%08X", m_HashValue);
}

unsigned StringHash::Calculate(std::string_view str)
{
    const char* ptr = str.data();
    unsigned hash = 0;
    while (*ptr)
    {
        hash = std::tolower(*ptr) + (hash << 6) + (hash << 16) - hash;
        ++ptr;
    }
    return hash;

}
} // namespace Pt