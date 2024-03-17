#include "VertexBuffer.hpp"

#include <glad/glad.h>

#include "GraphicsDefs.hpp"

namespace Pt {

VertexBuffer::VertexBuffer()
{

}

VertexBuffer::~VertexBuffer()
{

}

bool VertexBuffer::Create(const void* data)
{
    glGenBuffers(1, &buffer);

    return true;
}

} // namespace Pt