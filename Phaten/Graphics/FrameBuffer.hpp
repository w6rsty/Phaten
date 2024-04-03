#pragma once

#include "Object/Ptr.hpp"
#include "Math/IntVector.hpp"
#include "Texture.hpp"

namespace Pt {

class FrameBuffer : public RefCounted
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void Define(Texture* colorTex, Texture* depthStencilTex);

    void Bind();

    unsigned GLHandle() const { return m_Handle; }

    static void Bind(FrameBuffer* buffer);
    static void Unbind();
private:
    void Release();

    unsigned m_Handle;
    IntV2 m_Size;
};

} // namespace Pt