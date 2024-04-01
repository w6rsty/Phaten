#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "Graphics/GraphicsDefs.hpp"
#include "IO/Logger.hpp"
#include "ImGuiPlugin.hpp"

namespace Pt {

struct GraphicsData
{
    static constexpr float vertices[] = {
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 5
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 6
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 7
    };

    // cube indices
    static constexpr unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        1, 5, 6, 6, 2, 1,
        0, 4, 7, 7, 3, 0,
        0, 4, 5, 5, 1, 0,
        3, 7, 6, 6, 2, 3
    };

    Matrix4 model {1.0f};
};

static GraphicsData s_Data;

Application::Application() :
    m_Running(false),
    m_RenderState(false)
{
    m_Window = CreateShared<Window>(WindowCreateInfo{"Phaten", IntV2{1280, 720}, ScreenMode::WINDOWED});

    m_Input = CreateScoped<Input>();
    m_Input->SetOnExit([this]() {
        std::lock_guard<std::mutex> lock(m_RenderStateMutex);
        m_RenderState = false;
        m_Running = false;
    });
}

Application::~Application()
{
}

void Application::Run()
{
    m_Running = true;

    m_RenderThread = std::thread(&Application::OnRender, this);
    OnEvent();

    m_RenderThread.join();

    m_CameraController->Detach();
}

void Application::OnEvent()
{
    PT_TAG_INFO("Thread", "Launch OnEvent() on thread: ", std::this_thread::get_id());

    while (m_Running)
    {
        m_Input->Update();

        float speed =m_DeltaTime;
        if (m_Input->KeyDown(SDLK_LSHIFT))
            speed *= 2.0f;

        if (m_Input->KeyDown(SDLK_w))
            m_CameraController->Move(SceneCameraMovement::FORWARD, speed);
        if (m_Input->KeyDown(SDLK_s))
            m_CameraController->Move(SceneCameraMovement::BACKWARD, speed);
        if (m_Input->KeyDown(SDLK_a))
            m_CameraController->Move(SceneCameraMovement::LEFT, speed);
        if (m_Input->KeyDown(SDLK_d))
            m_CameraController->Move(SceneCameraMovement::RIGHT, speed);

        SDL_Delay(8);
    }
}

void Application::OnRender()
{
    PT_TAG_INFO("Thread", "Launch OnRender() on thread: ", std::this_thread::get_id());

    {
        m_Graphics = CreateScoped<Graphics>(m_Window);
        m_Graphics->SetVSync(true);
        m_Frequency = (double)SDL_GetPerformanceFrequency();
        ImGuiInit();
    
        m_Graphics->SetClearColor();

        m_VB = CreateShared<VertexBuffer>();
        m_VB->Define(BufferUsage::STATIC, 8, {VertexLayout{
            {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
            {VertexElementType::FLOAT2, VertexElementSemantic::TEXCOORD}
        }}, s_Data.vertices);
        m_VB->Bind(m_VB->Attributes());

        m_IB = CreateShared<IndexBuffer>();
        m_IB->Define(BufferUsage::STATIC, 36, s_Data.indices);
        m_IB->Bind();

        m_UB = CreateShared<UniformBuffer>();
        m_UB->Define(BufferUsage::DYNAMIC, sizeof(Matrix4));

        m_Graphics->LoadShader("Basic");
        m_Program = m_Graphics->CreateProgram("Basic", "", "");
        
        m_Texture = CreateShared<Texture2D>();
        m_Texture->Define("Assets/Textures/face.jpg");
        m_Texture->Bind();  

        m_Camera = CreateShared<Camera>();
        m_Camera->SetPerspective(45.0f, 0.1f, 100.0f);
        m_Camera->SetPosition(Vector3(0.0f, 0.0f, 3.0f));

        m_CameraController = CreateShared<SceneCameraController>();
        m_CameraController->Attach(m_Camera);

        {
            std::lock_guard<std::mutex> lock(m_RenderStateMutex);
            m_RenderState = true;
        }
    }

    SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    while (m_RenderState & m_Running)
    {
        uint64_t currentTime = SDL_GetPerformanceCounter();
        m_DeltaTime = (currentTime - m_LastTime) / m_Frequency;
        m_LastTime = currentTime;
        m_FPS = 1.0 / m_DeltaTime;

        m_Graphics->Clear();

        m_UB->Bind(0);
        Matrix4 mat = m_Camera->GetProjection() * m_Camera->GetView();
        m_UB->SetData(0, sizeof(Matrix4), &mat);

        m_Program->Bind();
        m_Graphics->SetUniform(m_Program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 36);

        ImGuiBegin();
        ImGui::Begin("Camera");
        ImGui::Text("Position: %.2f, %.2f, %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text("Rotation: %.2f, %.2f, %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
        ImGui::Text("FPS: %.2f", m_FPS);
        ImGui::End();
        ImGuiEnd();

        // Call window to swap buffers.
        m_Graphics->Present();
    }

    ImGuiShutdown();
}

} // namespace Pt