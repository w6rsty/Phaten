// TODO: Multithreading
#pragma once

#include "Input/Window.hpp"
#include "Input/Input.hpp"
#include "Scene/SceneCameraController.hpp"

namespace Pt {

class Application
{
public:
    Application();
    ~Application();

    void Run();
    
    void OnRender();

    static Vector2 sWindowSize;
private:
    SharedPtr<Window> m_Window;
    ScopedPtr<Input> m_Input;

    // Temporary data for rendering.
    SharedPtr<Camera> m_Camera;
    SharedPtr<SceneCameraController> m_CameraController;

    /// Render state.
    bool m_RenderState;

    double m_DeltaTime;
    // FPS
    double m_Frequency;
    uint64_t m_LastTime = 0.0f;
    float m_FPS = 0;
};

} // namespace Ptd