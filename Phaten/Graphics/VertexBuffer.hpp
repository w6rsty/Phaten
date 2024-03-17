#pragma once

#include "Core/Ptr.hpp"

namespace Pt {

class VertexBuffer : public RefCounted
{
public:
    VertexBuffer();
    ~VertexBuffer();
private:
    bool Create(const void* data);

    unsigned buffer;
    size_t numVertices;
    size_t vertexSize;

};

} // namespace Pt