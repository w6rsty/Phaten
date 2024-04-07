#pragma once

#include <string_view>

#include "Math/IntVector.hpp"
#include "Graphics/GraphicsDefs.hpp"

namespace Pt {

/// Reusable image class.
class Image : public RefCounted
{
public:
    Image();
    virtual ~Image();

    virtual void Load(std::string_view path);

    virtual const IntV2& Size() const { return m_Size; }
    ImageFormat Format() const { return m_Format; }
    unsigned char PixelBytes() const { return m_PixelBytes; }
    const unsigned char* Data() const { return m_Data; }
    const size_t Bytes() const { return m_Size.x * m_Size.y * m_PixelBytes; }
protected:
    virtual void Release();

    IntV2 m_Size;
    ImageFormat m_Format;
    unsigned char m_PixelBytes;
    unsigned char* m_Data;
};

class CubeMapImage : public Image
{
public:
    CubeMapImage();
    virtual ~CubeMapImage() override;

    virtual void Load(std::string_view path) override;
private:
    virtual void Release() override;
};

} // namespace Pt