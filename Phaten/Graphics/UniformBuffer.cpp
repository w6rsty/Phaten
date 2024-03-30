#include "UniformBuffer.hpp"

#include <glad/glad.h>

#include "IO/Logger.hpp"

namespace Pt {

/// Store all uniform buffer binding point.
static UniformBuffer* boundUniformBuffers[MAX_UNIFORM_BUFFER_SLOTS] {nullptr};

UniformBuffer::UniformBuffer() :
    m_Handle(0),
    m_Usage(BufferUsage::STATIC),
    m_SizeByte(0)
{
    // TODO: Ensure graphics system loaded.
}

UniformBuffer::~UniformBuffer()
{
    // TODO: Ensure graphics system loaded.
    Release();
}

bool UniformBuffer::Define(BufferUsage usage, size_t sizeByte, const void* data)
{
    Release();

    if (!sizeByte)
    {
        PT_LOG_ERROR("No uniform data, you fool");
        return false;
    }

    m_Usage = usage;
    m_SizeByte = sizeByte;

    return Create(data);
}

bool UniformBuffer::SetData(size_t offsetByte, size_t sizeByte, const void *data, bool discard)
{
    if (!m_Handle)
    {
        PT_LOG_ERROR("Uniform buffer has no been created");
        return false;
    }
    if (!data)
    {
        PT_LOG_ERROR("Uniform data is null, you fool!");
        return false;
    }
    if (offsetByte + sizeByte > m_SizeByte)
    {
        PT_LOG_ERROR("Setting uniform data out of range");
        return false;
    }


    GLenum usage = m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
    if (sizeByte == m_SizeByte)
    {
        glBufferData(GL_UNIFORM_BUFFER, m_SizeByte, data, usage);
    }
    else
    {
        if (discard)
        {
            glBufferData(GL_UNIFORM_BUFFER, m_SizeByte, nullptr, usage);
        }

        glBufferSubData(GL_UNIFORM_BUFFER, offsetByte, sizeByte, data);
    }

    return true;
}

void UniformBuffer::Bind(size_t index)
{
    if (!m_Handle || boundUniformBuffers[index] == this)
    {
        return;
    }

    glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(index), m_Handle, 0, m_SizeByte);
    boundUniformBuffers[index] = this;
}

void UniformBuffer::Unbind(size_t index)
{
    if (boundUniformBuffers[index])
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, static_cast<GLuint>(index), 0, 0, 0);
        boundUniformBuffers[index] = nullptr;
    }
}

bool UniformBuffer::Create(const void* data)
{
    glGenBuffers(1, &m_Handle);
    if (!m_Handle)
    {
        PT_LOG_ERROR("Failed to create uniform buffer");
        return false;
    }

    glBindBuffer(GL_UNIFORM_BUFFER, m_Handle);
    glBufferData(
        GL_UNIFORM_BUFFER,
        m_SizeByte,
        data,
        m_Usage == BufferUsage::DYNAMIC ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
    );
    
    PT_LOG_INFO("Created uniform buffer(SizeByte: ", m_SizeByte, ")");

    return true;
}

void UniformBuffer::Release()
{
    if (m_Handle)
    {
        glDeleteBuffers(1, &m_Handle);
        m_Handle = 0;

        for (UniformBuffer* buffer : boundUniformBuffers)
        {
            if (buffer == this)
            {
                buffer = nullptr;
            }
        }
    }
}

} // namespace Pt