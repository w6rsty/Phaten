#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "ImGuiPlugin.hpp"
#include "IO/Logger.hpp"

#include "Graphics/GraphicsDefs.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Graphics/Texture.hpp"

#include "Math/Transform.hpp"
#include "Math/Quaternion.hpp"

#include "Resource/Image.hpp"
#include "Resource/Mesh/BasicMesh.hpp"

namespace Pt {

struct GraphicsData
{
    Matrix4 model = ScaleMatrix4(0.2f);
};

static GraphicsData s_Data;

Application::Application() :
    m_Running(false),
    m_RenderState(false)
{
    m_Window = CreateShared<Window>(
        WindowCreateInfo{"Phaten", IntV2{1280, 720}, ScreenMode::WINDOWED});

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
    PT_TAG_DEBUG("Thread", "Launch OnEvent() on thread: ", std::this_thread::get_id());

    while (m_Running)
    {
        m_Input->Update();

        float speed = m_DeltaTime;
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

        unsigned eventSync = m_DeltaTime * 1000;
        unsigned sleep = eventSync < 5000 ? eventSync : 8;
        sleep = (sleep + (sleep >> 1)) >> 1;
        SDL_Delay(sleep);
    }
}

void Application::OnRender()
{
    PT_TAG_DEBUG("Thread", "Launch OnRender() on thread: ", std::this_thread::get_id());

    m_Graphics = CreateScoped<Graphics>(m_Window);
    m_Input->SetOnExit([this]() {
        m_RenderState = false;
        m_Running = false;
    });

    ImGuiInit();
    m_Input->SetPluginUpdate([this](const SDL_Event& event) {
        ImGuiProcessEvent(event);
    });

    m_Frequency = (double)SDL_GetPerformanceFrequency();

    auto vbo = CreateShared<VertexBuffer>();
    vbo->Define(BufferUsage::STATIC, Cube::VertexCount, {VertexLayout{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
        {VertexElementType::FLOAT3, VertexElementSemantic::NORMAL},
        {VertexElementType::FLOAT2, VertexElementSemantic::TEXCOORD}
    }}, Cube::Vertices);

    auto ibo = CreateShared<IndexBuffer>();
    ibo->Define(BufferUsage::STATIC, Cube::IndexCount, Cube::Indices);

    auto ubo = CreateShared<UniformBuffer>();
    ubo->Define(BufferUsage::DYNAMIC, sizeof(Matrix4) * 2);

    auto program = m_Graphics->CreateProgram("Basic", "", "");
    auto envProgram = m_Graphics->CreateProgram("Skybox", "", "");

    auto face = CreateScoped<Image>();
    face->Load("Assets/Textures/diamond_ore.png");

    auto faceTex = CreateShared<Texture>();
    faceTex->Define(TextureType::TEX_2D, face->Size(), face->Format(), face->Data());
    faceTex->SetWrapMode(0, TextureWrapMode::REPEAT);
    faceTex->SetWrapMode(1, TextureWrapMode::REPEAT);
    faceTex->SetFilterMode(TextureFilterMode::LINEAR);  

    auto environmentMap = CreateScoped<CubeMapImage>();
    environmentMap->Load("Assets/Environment/square_cubemap_512.png");

    auto envTex = CreateShared<Texture>();
    envTex->Define(TextureType::TEX_CUBE, environmentMap->Size(), environmentMap->Format(), environmentMap->Data());
    envTex->SetWrapMode(0, TextureWrapMode::CLAMP_TO_EDGE);
    envTex->SetWrapMode(1, TextureWrapMode::CLAMP_TO_EDGE);
    envTex->SetWrapMode(2, TextureWrapMode::CLAMP_TO_EDGE);
    envTex->SetFilterMode(TextureFilterMode::LINEAR);

    m_Camera = CreateShared<Camera>();
    m_Camera->SetPerspective(60.0f, 0.1f, 100.0f);
    m_Camera->SetPosition({0.0f, 0.0f, 3.0f});

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

        ubo->Bind(0);
        ubo->SetData(0, sizeof(Matrix4), m_Camera->GetProjection().Data());
        ubo->SetData(sizeof(Matrix4), sizeof(Matrix4), m_Camera->GetView().Data());
        /// ====================================================================
        m_Graphics->Clear(BufferBitType::COLOR | BufferBitType::DEPTH);

        glDepthMask(GL_FALSE);
        vbo->Bind(vbo->Attributes());
        ibo->Bind();
        envTex->Bind(0);
        envProgram->Bind();
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, Cube::IndexCount);
        glDepthMask(GL_TRUE);
        /// ====================================================================
        vbo->Bind(vbo->Attributes());
        ibo->Bind();
        faceTex->Bind(0);
        program->Bind();
        m_Graphics->SetUniform(program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, Cube::IndexCount);
        /// ====================================================================
        ImGuiBegin();
        ImGui::Begin("Settings");
        bool vsync = m_Graphics->IsVSync();
        if (ImGui::Checkbox("VSync", &vsync))
        {
            m_Graphics->SetVSync(vsync);
        } 
        ImGui::Separator();
        ImGui::Text("Position: %.2f, %.2f, %.2f",
            m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text("FPS: %.2f", m_FPS);
        ImGui::End();
        ImGuiEnd();

        // Call window to swap buffers.
        m_Graphics->Present();
    }

    ImGuiShutdown();
}

} // namespace Pt