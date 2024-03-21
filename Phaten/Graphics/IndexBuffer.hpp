#pragma once

#include "Core/Ptr.hpp"
#include "GraphicsDefs.hpp"

namespace Pt
{

class IndexBuffer : public RefCounted
{
public:
    IndexBuffer();
    ~IndexBuffer();

    bool Define(BufferUsage usage, size_t numIndices, const void* data = nullptr);
    /// Update buffer data, dicard means discard all old data.
    bool SetData(size_t startIdx, size_t numIndices, const void* data, bool discard = false);
    /// Bind index buffer.
    void Bind();

    /// Get OpenGL object handle.
    unsigned GLHandle() const { return m_Handle; }
    BufferUsage Usage() const { return m_Usage; }
    bool IsDynamic() const { return m_Usage == BufferUsage::DYNAMIC; }
    size_t NumIndices() const { return m_NumIndices; }
    
    static size_t BoundIndexBufferCount();
private:
    /// Create OpenGL buffer
    bool Create(const void* data);
    /// Release OpenGL resources
    void Release();

    unsigned m_Handle;
    BufferUsage m_Usage;
    size_t m_NumIndices;
};

} // namesoace Pt