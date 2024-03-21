#pragma once

#include <vector>

#include "Core/Ptr.hpp"
#include "Vertex.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

class VertexBuffer : public RefCounted
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool Define(BufferUsage usage, size_t numVertices, const VertexLayout& layout, const void* data = nullptr);
    /// Update buffer data, dicard means discard all old data.
    bool SetData(size_t startIdx, size_t numbVertices, const void* data, bool discard = false);
    /// Bind certain attributes
    /// Do not use 0.
    void Bind(unsigned attributeMask);

    /// Get OpenGL object identifier
    unsigned GLHandle() const { return m_Handle; }
    BufferUsage Usage() const { return m_Usage; }
    bool IsDynamic() const { return m_Usage == BufferUsage::DYNAMIC; }
    size_t NumVertices() const { return m_NumVertices; }
    unsigned Attributes() const { return m_EnabledAttributes; } 

    /// Use buffer layout to calculate enabled attributes to a mask
    static unsigned CalculateAttributesMask(const VertexLayout& layout);
private:
    /// Create OpenGL buffer
    bool Create(const void* data);
    /// Release OpenGL resources
    void Release();  

    /// OpenGL object identifier
    unsigned m_Handle;
    /// Usage of the buffer(static/dynamic)
    BufferUsage m_Usage;
    /// Total number of vertices
    size_t m_NumVertices;
    /// Vertex buffer layout
    VertexLayout m_Layout;
    /// Enbabled vertex attributes
    unsigned m_EnabledAttributes;
};

} // namespace Pt