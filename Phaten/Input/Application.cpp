#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "Graphics/GraphicsDefs.hpp"
#include "IO/Logger.hpp"
#include "Math/Space.hpp"
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

    SDL_Event event;
    while (m_Running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                std::lock_guard<std::mutex> lock(m_RenderStateMutex);
                m_RenderState = false;  
                m_Running = false;
            }
            
            if (m_RenderState)
            {
                m_Camera->OnEvent(event);
                ImGuiProcessEvent(event);
            }
        }
    }
}

void Application::OnRender()
{
    PT_TAG_INFO("Thread", "Launch OnRender() on thread: ", std::this_thread::get_id());

    {
        m_Graphics = CreateScoped<Graphics>(m_Window);
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

        m_Camera = CreateShared<SceneCamera>();
        m_Camera->SetPerspective(45.0f, 0.1f, 100.0f);
        m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

        {
            std::lock_guard<std::mutex> lock(m_RenderStateMutex);
            m_RenderState = true;
        }
    }

    SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    while (m_RenderState & m_Running)
    {
        m_Graphics->Clear();

        m_UB->Bind(0);
        Matrix4 mat = m_Camera->GetProjection() * m_Camera->GetView();
        m_UB->SetData(0, sizeof(Matrix4), &mat);

        m_Program->Bind();
        m_Graphics->SetUniform(m_Program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 36);

        ImGuiBegin();
        ImGui::ShowDemoWindow();
        ImGuiEnd();

        // Call window to swap buffers.
        m_Graphics->Present();
    }

    ImGuiShutdown();
}

} // namespace Pt