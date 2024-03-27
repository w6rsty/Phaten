#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "Math/Matrix.hpp"
#include "Math/Space.hpp"
#include "Input/ImGuiPlugin.hpp"

namespace Pt {

struct GraphicsData
{
    static constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    static constexpr unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    const Matrix4 projection = Perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    const Matrix4 view = LookAt(Vector3{0.0f, 0.0f, 3.0f}, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f});
};

static GraphicsData s_Data;

Application::Application()
{
    m_Graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED});

    ImGuiInit();

    m_VB = CreateShared<VertexBuffer>();
    m_VB->Define(BufferUsage::STATIC, 4, {{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
    }}, s_Data.vertices);
    m_VB->Bind(m_VB->Attributes());

    m_IB = CreateShared<IndexBuffer>();
    m_IB->Define(BufferUsage::STATIC, 6, s_Data.indices);
    m_IB->Bind();

    m_Graphics->LoadShader("Basic");
    m_Program = m_Graphics->CreateProgram("Basic", "", "");
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

            ImGuiProcessEvent(event);
        }

        ImGuiBegin();

        ImGui::ShowDemoWindow();

        m_Program->Bind();
        m_Graphics->SetUniform(m_Program, PresetUniform::U_WORLD_MATRIX, Matrix4::IDENTITY);
        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 6);

        ImGuiEnd();

        m_Graphics->Present();
    }
}

} // namespace Pt