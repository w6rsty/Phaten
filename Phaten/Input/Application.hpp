#pragma once

#include <thread>

#include "Object/Ptr.hpp"
#include "Input/Window.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Scene/SceneCamera.hpp"

namespace Pt {

class Application
{
public:
    Application();
    ~Application();

    void Run();
    
    void OnEvent();
    void OnRender();
private:
    SharedPtr<Window> m_Window;

    ScopedPtr<Graphics> m_Graphics;

    // Temporary data for rendering.
    SharedPtr<VertexBuffer> m_VB;
    SharedPtr<IndexBuffer> m_IB;
    SharedPtr<UniformBuffer> m_UB;
    SharedPtr<ShaderProgram> m_Program;
    ScopedPtr<SceneCamera> m_Camera;
    SharedPtr<Texture2D> m_Texture;

    /// Application state.
    bool m_Running;
    /// Render state.
    bool m_RenderState;

    std::thread m_RenderThread;
};

} // namespace Pt