#pragma once

#include <thread>
#include <mutex>

#include "Input/Window.hpp"
#include "Graphics/Graphics.hpp"
#include "Input/Input.hpp"
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
    SharedPtr<Camera> m_Camera;
    SharedPtr<SceneCameraController> m_CameraController;

    /// Applicat state.
    bool m_Running;
    /// Render state.
    bool m_RenderState;

    std::thread m_RenderThread;
    std::mutex m_RenderStateMutex;

    double m_DeltaTime;
    // FPS
    double m_Frequency;
    uint64_t m_LastTime = 0.0f;
    int m_FrameCount = 0;
    float m_FPS = 0;
};

} // namespace Ptd