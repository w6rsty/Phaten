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
        Release();
        return;
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

CubeMapImage::CubeMapImage() :
    Image()
{
}

CubeMapImage::~CubeMapImage()
{
    Release();
}

void CubeMapImage::Load(std::string_view path)
{
    Release();

    stbi_set_flip_vertically_on_load(false);

    int channels = 0;

    unsigned char* image = stbi_load(path.data(), &m_Size.x, &m_Size.y, &channels, 0);

    if (!image || (m_Size.x | m_Size.y | channels) == 0)
    {
        PT_LOG_WARN("Failed to load image: ", path);
        Release();
        return;
    }
    
    int size = m_Size.x / 4;
    m_Size.x = size;
    m_Size.y = size;

    m_Data = new unsigned char[size * size * channels * 6];

    // Copy image data to cubemap
    // right
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = (i + size) * size * channels * 4 + size * channels * 2;
        memcpy(m_Data + i * size * channels, image + lineBegin, size * channels);
    }
    // left
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = (i + size) * size * channels * 4;
        memcpy(m_Data + size * size * channels + i * size * channels, image + lineBegin, size * channels);
    }
    // top
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = i * size * channels * 4 + size * channels;
        memcpy(m_Data + size * size * channels * 2 + i * size * channels, image + lineBegin, size * channels);
    }
    // bottom
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = (i + size * 2) * size * channels * 4 + size * channels; 
        memcpy(m_Data + size * size * channels * 3 + i * size * channels, image + lineBegin, size * channels);
    }
    // back
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = (i + size) * size * channels * 4 + size * channels;
        memcpy(m_Data + size * size * channels * 4 + i * size * channels, image + lineBegin, size * channels);
    }
    // front
    for (int i = 0; i < size; ++i)
    {
        int lineBegin = (i + size) * size * channels * 4 + size * channels * 3;
        memcpy(m_Data + size * size * channels * 5 + i * size * channels, image + lineBegin, size * channels);
    }

    stbi_image_free(image);

    m_PixelBytes = channels;
    m_Format = PixelByteToImageFormat[channels];
    PT_LOG_INFO("Loaded cubemap image: ", path, " ", m_Size.x, "x", m_Size.y, " pixel bytes: ", (int)m_PixelBytes);
}

void CubeMapImage::Release()
{
    if (m_Data)
    {
        delete[] m_Data;
        m_Data = nullptr;
    }

    m_Size = IntV2::ZERO;
    m_Format = ImageFormat::NONE;
    m_PixelBytes = 0;
}

} // namespace Pt