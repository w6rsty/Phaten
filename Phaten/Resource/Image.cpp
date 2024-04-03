#include "Image.hpp"

#include <stb_image.h>

#include "IO/Logger.hpp"
#include "Graphics/GraphicsDefs.hpp"

namespace Pt {

static const ImageFormat PixelByteToImageFormat[] =
{
    ImageFormat::NONE,
    ImageFormat::R8,
    ImageFormat::RG8,
    ImageFormat::RGB8,
    ImageFormat::RGBA8
};

Image::Image() :
    m_Size(IntV2::ZERO),
    m_Format(ImageFormat::NONE),
    m_PixelBytes(0),
    m_Data(nullptr)
{
}

Image::~Image()
{
    Release();
}

void Image::Load(std::string_view path)
{
    Release();

    stbi_set_flip_vertically_on_load(true);

    int channels = 0;

    m_Data = stbi_load(path.data(), &m_Size.x, &m_Size.y, &channels, 0);

    if (!m_Data || (m_Size.x | m_Size.y | channels) == 0)
    {
        PT_LOG_WARN("Failed to load image: ", path);
    }

    m_PixelBytes = channels;
    m_Format = PixelByteToImageFormat[channels];
    PT_LOG_INFO("Loaded image: ", path, " ", m_Size.x, "x", m_Size.y, " pixel bytes: ", (int)m_PixelBytes);
}

void Image::Release()
{
    if (m_Data)
    {
        stbi_image_free(m_Data);
        m_Data = nullptr;
    }

    m_Size = IntV2::ZERO;
    m_Format = ImageFormat::NONE;
    m_PixelBytes = 0;
}


} // namespace Pt