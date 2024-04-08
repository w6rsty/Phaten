#include "Texture.hpp"

#include <glad/glad.h>

#include "Graphics/GraphicsDefs.hpp"
#include "IO/Logger.hpp"

namespace Pt {

static size_t ImageFormatToPixelByte[] =
{
    0,
    1,
    2,
    3,
    4
};

static const Texture* boundTextureSlot[MAX_TEXTURE_SLOTS] = { nullptr };

Texture::Texture() :
    m_Handle(0),
    m_Target(0),
    m_Size(IntV3::ZERO),
    m_Format(ImageFormat::NONE),
    m_Type(TextureType::TEX_2D),
    m_WrapModes{TextureWrapMode::REPEAT, TextureWrapMode::REPEAT, TextureWrapMode::REPEAT},
    m_FilterMode(TextureFilterMode::LINEAR)
{
    // Check graphics loaded
}

Texture::~Texture()
{
    Release();
}

void Texture::RegisterObject()
{
    Object::RegisterFactory<Texture>();
}

void Texture::Define(TextureType type, const IntV2& size, ImageFormat format, const void* data)
{
    Define(type, IntV3 {size, 1}, format, data);
}

void Texture::Define(TextureType type, const IntV3& size, ImageFormat format, const void* data)
{
    Release();

    m_Size = size;
    m_Type = type;
    m_Format = format;
    m_Target = TextureTypeGLTarget[EnumAsIndex(m_Type)];

    Create(data);
}

void Texture::Define(TextureType type, const SharedPtr<Image>& image)
{
    Define(type, image->Size(), image->Format(), image->Data());
}

void Texture::SetData(const void* data)
{
    if (!m_Handle)
    {
        return;
    }

    ForceBind();

    GLenum format = ImageFormatGLFormat[EnumAsIndex(m_Format)];

    glTexSubImage2D(m_Target, 0, 0, 0, m_Size.x, m_Size.y, format,
        ImageFormatGLDataType[EnumAsIndex(m_Format)], data);
}

void Texture::Bind(size_t index) const
{
    if (!m_Handle)
    {
        return;
    }

    if (index >= MAX_TEXTURE_SLOTS)
    {
        PT_LOG_WARN("Exceeded max texture slots. (", MAX_TEXTURE_SLOTS, ") get: ", index);
        return;
    }

    if (boundTextureSlot[index] == this)
    {
        return;
    }

    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(m_Target, m_Handle);

    boundTextureSlot[index] = this;
}

void Texture::SetWrapMode(size_t index, TextureWrapMode mode)
{
    if (!m_Handle)
    {
        return;
    }

    ForceBind();

    m_WrapModes[index] = mode;

    glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[0])]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[1])]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[2])]);
}

void Texture::SetFilterMode(TextureFilterMode mode)
{
    if (!m_Handle)
    {
        return;
    }

    ForceBind();

    m_FilterMode = mode;

    glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, TextureFilterModeGLType[EnumAsIndex(m_FilterMode)]);
}

void Texture::ForceBind() const
{
    boundTextureSlot[0] = nullptr;
    Bind(0);
}

bool Texture::Create(const void* data)
{
    glGenTextures(1, &m_Handle);
    glBindTexture(m_Target, m_Handle);

    glTexParameteri(m_Target, GL_TEXTURE_WRAP_S, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[0])]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_T, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[1])]);
    glTexParameteri(m_Target, GL_TEXTURE_WRAP_R, TextureWrapModeGLType[EnumAsIndex(m_WrapModes[2])]);
    glTexParameteri(m_Target, GL_TEXTURE_MIN_FILTER, TextureFilterModeGLType[EnumAsIndex(m_FilterMode)]);
    glTexParameteri(m_Target, GL_TEXTURE_MAG_FILTER, TextureFilterModeGLType[EnumAsIndex(m_FilterMode)]);

    GLenum internalFormat = ImageFormatGLInternalFormat[EnumAsIndex(m_Format)];
    GLenum format = ImageFormatGLFormat[EnumAsIndex(m_Format)];

    if (m_Type == TextureType::TEX_2D)
    {
        glTexImage2D(m_Target, 0, internalFormat, m_Size.x, m_Size.y, 0, format,
            ImageFormatGLDataType[EnumAsIndex(m_Format)], data);

        PT_LOG_INFO("Created texture: ", m_Size.x, "x", m_Size.y);
        return true;
    }
    else if (m_Type == TextureType::TEX_3D)
    {
        glTexImage3D(m_Target, 0, internalFormat, m_Size.x, m_Size.y, m_Size.z, 0, format,
            ImageFormatGLDataType[EnumAsIndex(m_Format)], data);

        PT_LOG_INFO("Created texture: ", m_Size.x, "x", m_Size.y, "x", m_Size.z);
        return true;
    }
    else // cubemap
    {
        for (size_t i = 0; i < 6; ++i)
        {
            size_t offset = m_Size.x * m_Size.y * ImageFormatToPixelByte[EnumAsIndex(m_Format)] * i;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_Size.x, m_Size.y, 0, format,
                ImageFormatGLDataType[EnumAsIndex(m_Format)], reinterpret_cast<void*>((unsigned char*)data + offset));
        }

        PT_LOG_INFO("Created cubemap texture: ", m_Size.x, "x", m_Size.y);
        return true;
    }
}

void Texture::Release()
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

    m_Target = 0;
    m_Size = IntV3::ZERO;

    m_Format = ImageFormat::NONE;
    m_Type = TextureType::TEX_2D;
    m_WrapModes[0] = m_WrapModes[1] = m_WrapModes[2] = TextureWrapMode::REPEAT;
    m_FilterMode = TextureFilterMode::LINEAR;
}

} // namespace Pt