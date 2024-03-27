#pragma once

#include "Object/Ptr.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"

namespace Pt {

class Application
{
public:
    Application();
    ~Application();

    void Run();
private:
    ScopedPtr<Graphics> m_Graphics;
    SharedPtr<VertexBuffer> m_VB;
    SharedPtr<IndexBuffer> m_IB;
    SharedPtr<ShaderProgram> m_Program;
};

} // namespace Pt