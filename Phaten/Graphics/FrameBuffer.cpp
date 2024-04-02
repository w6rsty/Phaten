#include "FrameBuffer.hpp"

#include <glad/glad.h>

#include "Math/IntVector.hpp"

namespace Pt {

static FrameBuffer* boundFrameBuffer = nullptr;
FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
    Release();
}

void FrameBuffer::Define(int x, int y)
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

    m_Size = IntV2(x, y);

    glGenTextures(1, &m_ColorTex);
    glBindTexture(GL_TEXTURE_2D, m_ColorTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Size.x, m_Size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTex, 0);

    glGenTextures(1, &m_DepthStencilTex);
    glBindTexture(GL_TEXTURE_2D, m_DepthStencilTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Size.x, m_Size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthStencilTex, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        PT_LOG_ERROR("Framebuffer is not complete!");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    PT_LOG_INFO("Created framebuffer: width: ", m_Size.x, ", height: ", m_Size.y);
}

void FrameBuffer::Bind()
{
    if (!m_Handle || boundFrameBuffer == this)
    {
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
    glViewport(0, 0, m_Size.x, m_Size.y);
    boundFrameBuffer = this;
}

void FrameBuffer::Bind(FrameBuffer* buffer)
{
    if (boundFrameBuffer != buffer)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, buffer ? buffer->GLHandle() : 0);
        boundFrameBuffer = buffer;
    }
}

void FrameBuffer::Unbind()
{
    if (boundFrameBuffer)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        boundFrameBuffer = nullptr;
    }
}

void FrameBuffer::Release()
{
    if (m_Handle)
    {
        if (boundFrameBuffer == this)
        {
            FrameBuffer::Unbind();
        }

        glDeleteFramebuffers(1, &m_Handle);
        m_Handle = 0;
    }
}


} // namespace Pt