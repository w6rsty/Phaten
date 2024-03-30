#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "Graphics/GraphicsDefs.hpp"
#include "Input/ImGuiPlugin.hpp"
#include "Math/Space.hpp"
#include "Math/Vector.hpp"

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

Application::Application()
{
    m_Graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{1280, 720}, ScreenMode::WINDOWED});
    m_Graphics->SetClearColor();

    ImGuiInit();

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

    m_Camera = CreateScoped<SceneCamera>();
    m_Camera->SetPerspective(45.0f, 0.1f, 100.0f);
    m_Camera->SetPosition({ 0.0f, 0.0f, 3.0f });

    m_Texture = CreateShared<Texture2D>();
    m_Texture->Define("Assets/Textures/face.jpg");
    m_Texture->Bind();  
}

Application::~Application()
{
    ImGuiShutdown();
}

void Application::Run()
{
    bool running = true;
    SDL_Event event;

    while (running)
    {
        m_Graphics->Clear();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    m_Camera->Move(SceneCamera::Movement::FORWARD);
                    break;
                case SDLK_s:
                    m_Camera->Move(SceneCamera::Movement::BACKWARD);
                    break;
                case SDLK_a:
                    m_Camera->Move(SceneCamera::Movement::LEFT);
                    break;
                case SDLK_d:
                    m_Camera->Move(SceneCamera::Movement::RIGHT);
                    break;
                }
            }

            if (event.type == SDL_MOUSEMOTION && event.motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
            {
                m_Camera->Rotate(event.motion.xrel, -event.motion.yrel);
            }

            ImGuiProcessEvent(event);
        }

        ImGuiBegin();

        ImGui::Begin("Camera");
        ImGui::Text("Position: %.2f %.2f %.2f", m_Camera->GetPosition().x, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
        ImGui::Text("Rotation: %.2f %.2f %.2f", m_Camera->GetRotation().x, m_Camera->GetRotation().y, m_Camera->GetRotation().z);
        ImGui::End();

        m_UB->Bind(0);
        Matrix4 mat = m_Camera->GetProjection() * m_Camera->GetView();
        m_UB->SetData(0, sizeof(Matrix4), &mat);

        m_Program->Bind();
        m_Graphics->SetUniform(m_Program, PresetUniform::U_MODEL, s_Data.model);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 36);

        ImGuiEnd();

        m_Graphics->Present();
    }
}

} // namespace Pt