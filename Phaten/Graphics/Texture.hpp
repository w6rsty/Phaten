#pragma once

#include <string_view>

#include "Object/Ptr.hpp"
#include "Math/IntVector.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

/// Base class for all textures
class Texture : public RefCounted
{
public:
    // TODO: Support resource manager
    virtual ~Texture() = default;

    virtual IntV2 Size2D() const { return IntV2::ZERO; }

    virtual unsigned GLHandle() const = 0;
    virtual TextureType TexGLType() const = 0;
    virtual unsigned TexGLTarget() const = 0;
};

/// 2D texture
class Texture2D : public Texture
{
public:
    Texture2D();
    ~Texture2D();

    // TODO: Support resource manager
    /// Load texture from file
    void Define(std::string_view path, bool flip = true);
    /// Create texture
    void Define(int width, int height, int channels, const void* data);
    void Bind() const;

    void SetWrapMode(TextureWrapMode mode);
    void SetFilterMode(TextureFilterMode minMode, TextureFilterMode magMode);

    int Channels() const { return m_Channels; }
    virtual IntV2 Size2D() const override { return IntV2 {m_Width, m_Height}; }

    TextureWrapMode WrapMode() const { return m_WrapMode; }
    TextureFilterMode MinFilterMode() const { return m_MinFilterMode; }
    TextureFilterMode MagFilterMode() const { return m_MagFilterMode; }

    virtual unsigned GLHandle() const override { return m_Handle; }
    virtual TextureType TexGLType() const override { return TextureType::TEX_2D; }
    virtual unsigned TexGLTarget() const override;
private:
    bool Create(std::string_view path);
    bool Create(const void* data);
    void Release();

    unsigned m_Handle;
    bool m_Flip;
    int m_Width;
    int m_Height;
    int m_Channels;

    bool m_Loaded;

    TextureWrapMode m_WrapMode;
    TextureFilterMode m_MinFilterMode;
    TextureFilterMode m_MagFilterMode;
};

} // namespace Pt