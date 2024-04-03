#pragma once

#include <string_view>

#include "Math/IntVector.hpp"
#include "Graphics/GraphicsDefs.hpp"

namespace Pt {

/// Reusable image class.
class Image
{
public:
    Image();
    ~Image();

    void Load(std::string_view path);

    const IntV2& Size() const { return m_Size; }
    ImageFormat Format() const { return m_Format; }
    unsigned char PixelBytes() const { return m_PixelBytes; }
    const unsigned char* Data() const { return m_Data; }
private:
    void Release();

    IntV2 m_Size;
    ImageFormat m_Format;
    unsigned char m_PixelBytes;
    unsigned char* m_Data;
};

} // namespace Pt