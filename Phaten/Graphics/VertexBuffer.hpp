#pragma once

#include <vector>

#include "Core/Ptr.hpp"
#include "Graphics/GraphicsDefs.hpp"

namespace Pt {

struct VertexElement
{

};

class VertexLayout
{
public:
    size_t Size() const { return elements.size(); }
private:
    std::vector<VertexElement> elements;
};

class VertexBuffer : public RefCounted
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool Define(BufferUsage usage, size_t numVertices, const VertexLayout& layout, const void* data = nullptr);
    bool SetData();
    void Bind();

    size_t NumVertices() const { return numVertices; }
    size_t NumElements() const { return layout.Size(); }
    const VertexLayout& Layout() const { return layout; }
    size_t VertexSize() const { return vertexSize; }
    unsigned Attributes() const { return attributes; }
    BufferUsage Usage() const { return usage; }
    bool IsDynamic() const { return usage == BufferUsage::DYNAMIC; }

    /// Get buffer OpenGL object identifier
    unsigned GLBuffer() const { return buffer; }
private:
    bool Create(const void* data);
    bool Release();

    unsigned buffer;
    size_t numVertices;
    size_t vertexSize;
    unsigned attributes;
    BufferUsage usage;

    VertexLayout layout;
};

} // namespace Pt