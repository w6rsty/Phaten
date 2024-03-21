#pragma once

#include "Core/Ptr.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

class UniformBuffer : public RefCounted
{
public:
    UniformBuffer();
    ~UniformBuffer();

    bool Define(BufferUsage usage, size_t sizeByte, const void* data = nullptr);
    /// Update buffer data, dicard means discard all old data.
    bool SetData(size_t offsetByte, size_t sizeByte, const void* data, bool discard = false);
    /// Bind to a certain binding point.
    void Bind(size_t index);
    
    unsigned GLHandle() const { return m_Handle; }
    BufferUsage Usage() const { return m_Usage; }
    bool IsDynamic() const { return m_Usage == BufferUsage::DYNAMIC; }
    size_t SizeByte() const { return m_SizeByte; }

    /// Unbind from a certain binding point.
    static void Unbind(size_t index);
private:
    bool Create(const void* data);
    void Release();

    unsigned m_Handle;
    BufferUsage m_Usage;
    size_t m_SizeByte;
};

} // namespace Pt