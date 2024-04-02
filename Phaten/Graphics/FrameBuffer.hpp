#pragma once

#include "Object/Ptr.hpp"
#include "Graphics/Texture.hpp"

namespace Pt {

class FrameBuffer : public RefCounted
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void Define(Texture* colorTex, Texture* depthStencilTex);

    unsigned GLHandle() const { return m_Handle; }

    void Bind();

    static void Bind(FrameBuffer* draw, FrameBuffer* read);
    static void Unbind();
private:
    void Release();

    unsigned m_Handle;
};

} // namespace Pt