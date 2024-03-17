#include "Graphics/VertexBuffer.hpp"

#include "glad/glad.h"

namespace Pt {

bool VertexBuffer::Create(const void* data)
{
    glGenBuffers(1, &buffer);

    return true;
}

} // namespace Pt