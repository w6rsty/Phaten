#pragma once

#include "Object/Ptr.hpp"
#include "Math/IntVector.hpp"

namespace Pt {

class FrameBuffer : public RefCounted
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void Define(int x, int y);

    void Bind();

    unsigned GLHandle() const { return m_Handle; }

    unsigned ColorTexture() const { return m_ColorTex; }
    unsigned DepthStencilTexture() const { return m_DepthStencilTex; }

    static void Bind(FrameBuffer* buffer);
    static void Unbind();
private:
    void Release();

    unsigned m_Handle;
    IntV2 m_Size;
    unsigned m_ColorTex;
    unsigned m_DepthStencilTex;
};

} // namespace Pt