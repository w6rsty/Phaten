#pragma once

#include <thread>

#include "Object/Ptr.hpp"
#include "Input/Window.hpp"
#include "Input/Input.hpp"
#include "Graphics/Graphics.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Scene/SceneCameraController.hpp"

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
    ScopedPtr<Input> m_Input;

    // Temporary data for rendering.
    SharedPtr<VertexBuffer> m_VB;
    SharedPtr<IndexBuffer> m_IB;
    SharedPtr<UniformBuffer> m_UB;
    SharedPtr<ShaderProgram> m_Program;
    SharedPtr<Camera> m_Camera;
    SharedPtr<SceneCameraController> m_CameraController;
    SharedPtr<Texture2D> m_Texture;

    /// Applicat state.
    bool m_Running;
    /// Render state.
    bool m_RenderState;

    std::thread m_RenderThread;
    std::mutex m_RenderStateMutex;

    double m_Frequency;
    double m_DeltaTime;
    uint64_t m_LastTime = 0.0f;
    int m_FrameCount = 0;
    float m_FPS = 0;
};

} // namespace Ptd