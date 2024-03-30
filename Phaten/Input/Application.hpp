#pragma once

#include "Graphics/UniformBuffer.hpp"
#include "Object/Ptr.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Scene/SceneCamera.hpp"
#include "Graphics/Texture.hpp"

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
    SharedPtr<UniformBuffer> m_UB;
    SharedPtr<ShaderProgram> m_Program;

    ScopedPtr<SceneCamera> m_Camera;

    SharedPtr<Texture2D> m_Texture;
};

} // namespace Pt