#include "FrameBuffer.hpp"

#include <glad/glad.h>

#include "Math/IntVector.hpp"

namespace Pt {

static FrameBuffer* boundDrawBuffer = nullptr;
static FrameBuffer* boundReadBuffer = nullptr;

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_Handle);
}

FrameBuffer::~FrameBuffer()
{
    Release();
}

void FrameBuffer::Define(Texture* colorTex, Texture* depthStencilTex)
{
    Bind();

    IntV2 size = IntV2::ZERO;

    if (colorTex && colorTex->TexGLType() == TextureType::TEX_2D)
    {
        size = colorTex->Size2D();
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, colorTex->TexGLTarget(), colorTex->GLHandle(), 0);
    }
    else {
        glDrawBuffer(GL_NONE);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
    }


    if (depthStencilTex)
    {
        if (size != IntV2::ZERO && size != depthStencilTex->Size2D())
        {
            PT_LOG_WARN("Framebuffer color and depth dimensions don't match");
        }
        else
        {
            size = depthStencilTex->Size2D();
        }

        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthStencilTex->TexGLTarget(), depthStencilTex->GLHandle(), 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, depthStencilTex->TexGLTarget(), depthStencilTex->GLHandle(), 0);
    }
    else
    {
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
    }

    PT_LOG_INFO("Created framebuffer: width: ", size.x, ", height: ", size.y);
}

void FrameBuffer::Bind()
{
    if (!m_Handle || boundDrawBuffer == this)
    {
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
    boundDrawBuffer = this;
}

void FrameBuffer::Bind(FrameBuffer* draw, FrameBuffer* read)
{
    if (boundDrawBuffer != draw)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw ? draw->GLHandle() : 0);
        boundDrawBuffer = draw;
    }

    if (boundReadBuffer != read)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, read ? read->GLHandle() : 0);
        boundReadBuffer = read;
    }
}

void FrameBuffer::Unbind()
{
    if (boundDrawBuffer)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        boundDrawBuffer = nullptr;
    }

    if (boundReadBuffer)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        boundReadBuffer = nullptr;
    }
}

void FrameBuffer::Release()
{
    if (m_Handle)
    {
        if (boundDrawBuffer == this || boundReadBuffer == this)
        {
            FrameBuffer::Unbind();
        }

        glDeleteFramebuffers(1, &m_Handle);
        m_Handle = 0;
    }
}


} // namespace Pt