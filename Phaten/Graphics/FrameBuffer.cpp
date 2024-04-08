#include "FrameBuffer.hpp"

#include <glad/glad.h>

#include "Math/IntVector.hpp"

namespace Pt {

static FrameBuffer* boundFrameBuffer = nullptr;

FrameBuffer::FrameBuffer() :
    m_Handle(0),
    m_Size(IntV2::ZERO)
{
}

FrameBuffer::~FrameBuffer()
{
    Release();
}

void FrameBuffer::Define(Texture* colorTex, Texture* depthStencilTex)
{
    glGenFramebuffers(1, &m_Handle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

    m_Size = colorTex->Size2D();

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        colorTex->GLTarget(),
        colorTex->GLHandle(),
        0);

    if (depthStencilTex)
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            depthStencilTex->GLTarget(),
            depthStencilTex->GLHandle(),
            0);
    }
    /// FIXME: D16 and D32 not works.
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
    // FIXME: Viewport set.
    // glViewport(0, 0, m_Size.x, m_Size.y);
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

    m_Size = IntV2::ZERO;
}


} // namespace Pt