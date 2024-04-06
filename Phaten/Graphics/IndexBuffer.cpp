#include "IndexBuffer.hpp"

#include <glad/glad.h>

#include "IO/Logger.hpp"

namespace Pt {

static size_t boundIndexCount = 0;
static IndexBuffer* boundIndexBuffer = nullptr;

IndexBuffer::IndexBuffer() :
    m_Handle(0),
    m_Usage(BufferUsage::STATIC),
    m_NumIndices(0)
{
    // TODO: Ensure graphics system loaded.
}

IndexBuffer::~IndexBuffer()
{
    // TODO: Ensure graphics system loaded.
    Release();
}

bool IndexBuffer::Define(BufferUsage usage, size_t numIndices, const void* data)
{
    Release();

    if (!numIndices)
    {
        PT_LOG_ERROR("No indices, you fool");
        return false;
    }

    m_Usage = usage;
    m_NumIndices = numIndices;

    return Create(data);
}

bool IndexBuffer::SetData(size_t startIdx, size_t numIndices, const void *data, bool discard)
{
    if (!m_Handle)
    {
        PT_LOG_ERROR("Index buffer has not been created");
        return false;
    }
    if (!data)
    {   
        PT_LOG_ERROR("Index data is null, you fool!");
        return false;
    }
    if (startIdx + numIndices > m_NumIndices)
    {
        PT_LOG_ERROR("Invalid range of indices");
        return false;
    }

    Bind();

    size_t totalIndexSize = m_NumIndices * sizeof(unsigned);
    GLenum usage = m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    if (numIndices == m_NumIndices)
    {
        glBufferData(GL_ARRAY_BUFFER, totalIndexSize, data, usage);
    }
    else
    {
        if (discard)
        {
            glBufferData( GL_ARRAY_BUFFER, totalIndexSize, nullptr, usage);
        }

        glBufferSubData(
            GL_ARRAY_BUFFER,
            startIdx * sizeof(unsigned),
            numIndices * sizeof(unsigned), 
            data
        );
    }

    return true;
}

void IndexBuffer::Bind()
{
    if (!m_Handle || boundIndexBuffer == this)
    {
        return;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);

    boundIndexCount = m_NumIndices;
    boundIndexBuffer = this;
}

bool IndexBuffer::Create(const void* data)
{
    glGenBuffers(1, &m_Handle);
    if (!m_Handle)
    {
        PT_LOG_ERROR("Failed to create index buffer");
        return false;
    }

    Bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_NumIndices * sizeof(unsigned),
        data,
        m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
    );
    PT_LOG_INFO("Created index buffer(NumVertices: ", m_NumIndices, ")");

    return true;
}

void IndexBuffer::Release()
{
    if(m_Handle)
    {
        glDeleteBuffers(1, &m_Handle);
        m_Handle = 0;

        if (boundIndexBuffer == this)
        {
            boundIndexBuffer = nullptr;
            boundIndexCount = 0;
        }
    }
}

/// Return constant numbers of current bounded index count
size_t IndexBuffer::BoundIndexBufferCount()
{
    return boundIndexCount;
}

} // namespace Pt