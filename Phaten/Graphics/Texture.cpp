#include "Texture.hpp"

#include <glad/glad.h>
#include <stb_image.h>

#include "IO/Logger.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

static const Texture* boundTextureSlot[MAX_TEXTURE_SLOTS] = { nullptr };
static size_t boundTextureSlotIndex = 0;

Texture2D::Texture2D() :
    m_Handle(0),
    m_Flip(true),
    m_Width(0),
    m_Height(0),
    m_Channels(0),
    m_Loaded(false),
    m_WrapMode(TextureWrapMode::REPEAT),
    m_MinFilterMode(TextureFilterMode::LINEAR),
    m_MagFilterMode(TextureFilterMode::LINEAR)
{
    // Check graphics loaded
}

Texture2D::~Texture2D()
{
    Release();
}

void Texture2D::Define(std::string_view path, bool flip)
{
    Release();

    m_Flip = flip;

    if (Create(path))
    {
        m_Loaded = true;
    }
}

void Texture2D::Define(int width, int height, int channels, const void* data)
{
    Release();

    m_Flip = false;
    m_Width = width;
    m_Height = height;
    m_Channels = channels;

    if (Create(data))
    {
        m_Loaded = true;
    }
}

void Texture2D::Bind() const
{
    if (boundTextureSlotIndex >= MAX_TEXTURE_SLOTS)
    {
        PT_LOG_WARN("Exceeded max texture slots.");
        return;
    }

    for (size_t i = 0; i < MAX_TEXTURE_SLOTS; ++i)
    {
        if (boundTextureSlot[i] == this)
        {
            return;
        }
    }

    glActiveTexture(GL_TEXTURE0 + boundTextureSlotIndex);
    glBindTexture(GL_TEXTURE_2D, m_Handle);

    boundTextureSlot[boundTextureSlotIndex] = this;
    ++boundTextureSlotIndex;
}

void Texture2D::SetWrapMode(TextureWrapMode mode)
{
    Bind();

    m_WrapMode = mode;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
}

void Texture2D::SetFilterMode(TextureFilterMode minMode, TextureFilterMode magMode)
{
    Bind();

    m_MinFilterMode = minMode;
    m_MagFilterMode = magMode;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MinFilterMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MagFilterMode)]);
}

bool Texture2D::Create(std::string_view path)
{
    stbi_set_flip_vertically_on_load(m_Flip);

    unsigned char* data = stbi_load(path.data(), &m_Width, &m_Height, &m_Channels, 0);
    if (!data)
    {
        PT_LOG_WARN("Failed to load texture: ", path);
        return false;
    }

    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MinFilterMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MagFilterMode)]);

    GLenum format = GL_RGB;
    if (m_Channels == 4)
    {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    PT_LOG_INFO("Created texture. width: ", m_Width, ", height: ", m_Height, ", channels: ", m_Channels);

    return true;
}

bool Texture2D::Create(const void* data)
{
    glGenTextures(1, &m_Handle);
    glBindTexture(GL_TEXTURE_2D, m_Handle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TextureWrapModeGLType[EnumAsIndex(m_WrapMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MinFilterMode)]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TextureFilterModeGLType[EnumAsIndex(m_MagFilterMode)]);

    GLenum format = GL_RGB;
    if (m_Channels == 4)
    {
        format = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);

    PT_LOG_INFO("Created texture. width: ", m_Width, ", height: ", m_Height, ", channels: ", m_Channels);

    return true;
}

void Texture2D::Release()
{
    if (m_Handle)
    {
        glDeleteTextures(1, &m_Handle);
        m_Handle = 0;
        for (size_t i = 0; i < MAX_TEXTURE_SLOTS; ++i)
        {
            if (boundTextureSlot[i] == this)
            {
                boundTextureSlot[i] = nullptr;
            }
        }
    }

    m_Loaded = false;
    m_Width = 0;
    m_Height = 0;
    m_Channels = 0;
}

} // namespace Pt