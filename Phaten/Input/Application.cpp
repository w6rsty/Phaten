#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "IO/Logger.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/GraphicsDefs.hpp"
#include "ImGuiPlugin.hpp"
#include "Math/Matrix.hpp"
#include "Math/Quaternion.hpp"
#include "Resource/Image.hpp"

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

        if (m_Input->MouseButtonDown(3))
        {
            auto move = m_Input->MouseMove();
            m_CameraController->Rotate(move.x, move.y);
        }

        SDL_Delay(8);
    }
}

void Application::OnRender()
{
    PT_TAG_INFO("Thread", "Launch OnRender() on thread: ", std::this_thread::get_id());

    
    m_Graphics = CreateScoped<Graphics>(m_Window);
    m_Input->SetOnExit([this]() {
        m_RenderState = false;
        m_Running = false;
    });

    ImGuiInit();
    m_Input->SetPluginUpdate([this](const SDL_Event& event) {
        ImGuiProcessEvent(event);
    });

    m_Graphics->SetVSync(true);
    m_Graphics->SetClearColor();
    m_Frequency = (double)SDL_GetPerformanceFrequency();

    auto vbo = CreateShared<VertexBuffer>();
    vbo->Define(BufferUsage::STATIC, 8, {VertexLayout{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
        {VertexElementType::FLOAT2, VertexElementSemantic::TEXCOORD}
    }}, s_Data.vertices);
    vbo->Bind(vbo->Attributes());

    auto ibo = CreateShared<IndexBuffer>();
    ibo->Define(BufferUsage::STATIC, 36, s_Data.indices);
    ibo->Bind();

    auto ubo = CreateShared<UniformBuffer>();
    ubo->Define(BufferUsage::DYNAMIC, sizeof(Matrix4));

    auto fbo = CreateShared<FrameBuffer>();
    auto colorAttachment = CreateShared<Texture>();
    colorAttachment->Define(TextureType::TEX_2D, IntV2{1280, 720}, ImageFormat::RGB8, nullptr);
    auto depthAttachment = CreateShared<Texture>();
    depthAttachment->Define(TextureType::TEX_2D, IntV2{1280, 720}, ImageFormat::D24S8, nullptr);
    fbo->Define(colorAttachment, depthAttachment);

    m_Graphics->LoadShader("Basic");
    auto program = m_Graphics->CreateProgram("Basic", "", "");

    auto image = CreateScoped<Image>();
    image->Load("Assets/Textures/face.jpg");

    auto tex = CreateShared<Texture>();
    tex->Define(TextureType::TEX_2D, image->Size(), image->Format(), image->Data());

    m_Camera = CreateShared<Camera>();
    m_Camera->SetPerspective(45.0f, 0.1f, 100.0f);
    m_Camera->SetPosition(Vector3(0.0f, 0.0f, 3.0f));

    m_CameraController = CreateShared<SceneCameraController>();
    m_CameraController->Attach(m_Camera);

    {
        std::lock_guard<std::mutex> lock(m_RenderStateMutex);
        m_RenderState = true;
    }

    SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    while (m_RenderState & m_Running)
    {
        uint64_t currentTime = SDL_GetPerformanceCounter();
        m_DeltaTime = (currentTime - m_LastTime) / m_Frequency;
        m_LastTime = currentTime;
        m_FPS = 1.0 / m_DeltaTime;

        m_Graphics->SetFrameBuffer(fbo);
        m_Graphics->SetClearColor({0.3f, 0.3f, 0.3f, 1.0f});
        m_Graphics->Clear();

        ubo->Bind(0);
        Matrix4 mat = m_Camera->GetProjection() * m_Camera->GetView();
        ubo->SetData(0, sizeof(Matrix4), &mat);

        program->Bind();
        tex->Bind(0);
        m_Graphics->SetUniform(program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 36);

        m_Graphics->SetFrameBuffer(nullptr);
        m_Graphics->SetClearColor();
        m_Graphics->Clear();

        ubo->Bind(0);
        ubo->SetData(0, sizeof(Matrix4), &mat);

        program->Bind();
        
        colorAttachment->Bind(0);
        m_Graphics->SetUniform(program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 36);

        ImGuiBegin();
        ImGui::Begin("Camera");
        ImGui::Text("Position: %.2f, %.2f, %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text("Rotation: %.2f, %.2f, %.2f %.2f", m_Camera->GetRotation().w, m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
        ImGui::Text("FPS: %.2f", m_FPS);

        ImGui::Image(reinterpret_cast<void*>(colorAttachment->GLHandle()), ImVec2(320, 180), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGuiEnd();

        // Call window to swap buffers.
        m_Graphics->Present();
    }

    ImGuiShutdown();
}

} // namespace Pt