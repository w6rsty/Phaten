#include "Application.hpp"

#include <SDL.h>
#include <imgui.h>

#include "Math/Space.hpp"
#include "Input/ImGuiPlugin.hpp"

namespace Pt {

struct GraphicsData
{
    static constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    static constexpr unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    Vector3 camPos = {0, 0, 3};
    Vector3 camFront = {0, 0, -1};
    Vector3 camUp = {0, 1, 0};

    float camSpeed = 0.1f;

    Matrix4 view;
    Matrix4 projection;
};

static GraphicsData s_Data;

Application::Application()
{
    m_Graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{1200, 600}, ScreenMode::WINDOWED});

    ImGuiInit();

    m_VB = CreateShared<VertexBuffer>();
    m_VB->Define(BufferUsage::STATIC, 4, {VertexLayout{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
        {VertexElementType::FLOAT2, VertexElementSemantic::TEXCOORD}
    }}, s_Data.vertices);
    m_VB->Bind(m_VB->Attributes());

    m_IB = CreateShared<IndexBuffer>();
    m_IB->Define(BufferUsage::STATIC, 6, s_Data.indices);
    m_IB->Bind();

    m_UB = CreateShared<UniformBuffer>();
    m_UB->Define(BufferUsage::DYNAMIC, sizeof(Matrix4));

    m_Graphics->LoadShader("Basic");
    m_Program = m_Graphics->CreateProgram("Basic", "", "");
}

Application::~Application()
{
    ImGuiShutdown();
}

void Application::Run()
{
    LookAt(s_Data.view, s_Data.camPos, s_Data.camPos + s_Data.camFront, s_Data.camUp);
    Perspective(s_Data.projection, 45.0f, 2.0f, 0.1f, 100.0f);

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

        ImGui::Begin("Camera");
        if (ImGui::DragFloat3("Position", &s_Data.camPos.x, 0.1f))
        {
            LookAt(s_Data.view, s_Data.camPos, s_Data.camPos + s_Data.camFront, s_Data.camUp);
        }
        ImGui::End();

        m_Program->Bind();

        m_UB->Bind(0);
        Matrix4 viewProjection = s_Data.projection * s_Data.view;
        m_UB->SetData(0, sizeof(Matrix4), &viewProjection);

        m_Graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 6);

        ImGuiEnd();
        m_Graphics->Present();
    }
}

} // namespace Pt