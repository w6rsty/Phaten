#pragma once

#include <string_view>

#include "Object/Ptr.hpp"
#include "Object/Object.hpp"
#include "Math/IntVector.hpp"
#include "GraphicsDefs.hpp"
#include "Resource/Image.hpp"

namespace Pt {

// TODO: Support resource manager
/// Texture
class Texture : public Object
{
    OBJECT(Texture);
public:
    Texture();
    ~Texture();

    static void RegisterObject();

    /// Create texture
    void Define(TextureType type, const IntV2& size, ImageFormat format, const void* data);
    void Define(TextureType type, const IntV3& size, ImageFormat format, const void* data);
    void Define(TextureType type, const SharedPtr<Image>& image);
    void SetData(const void* data);
    /// Auto bind texture
    void Bind(size_t index) const;

    void SetWrapMode(size_t index, TextureWrapMode mode);
    void SetFilterMode(TextureFilterMode mode);

    /// Get texture2d size
    IntV2 Size2D() const { return IntV2 {m_Size.x, m_Size.y}; }
    /// Get texture wrap mode
    TextureWrapMode WrapMode(size_t index) const { return m_WrapModes[index]; }
    /// Get texture filter mode
    TextureFilterMode FilterMode() const { return m_FilterMode; }

    /// Get texture GL handle
    unsigned GLHandle() const { return m_Handle; }
    /// Get texture GL type
    TextureType GLType() const { return m_Type; }
    /// Get texture GL target
    unsigned GLTarget() const { return m_Target; }
    /// Get texture format
    ImageFormat Format() const { return m_Format; }
private:
    void ForceBind() const;

    bool Create(const void* data);
    void Release();

    unsigned m_Handle;
    /// Texture GL target(from type)
    unsigned m_Target;
    IntV3 m_Size;

    /// Internal storage format
    ImageFormat m_Format;
    // Texture type
    TextureType m_Type;
    TextureWrapMode m_WrapModes[3];
    TextureFilterMode m_FilterMode;
};

} // namespace Pt