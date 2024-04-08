#include "Application.hpp"

#include <SDL.h>

#include "IO/StringUtils.hpp"

#include "Graphics/UniformBuffer.hpp"

#include "Renderer/StaticGeometry.hpp"
#include "Renderer/TextRenderer.hpp"

namespace Pt {

Vector2 Application::sWindowSize {1280, 720};

Application::Application() :
    m_RenderState(false)
{
    m_Window = CreateShared<Window>(WindowCreateInfo{"Phaten", sWindowSize, ScreenMode::WINDOWED});

    m_Input = CreateScoped<Input>();
    m_Input->SetOnExit([this]() {
        m_RenderState = false;
    });
}

Application::~Application()
{
}

void Application::Run()
{
    OnRender();
}

void Application::OnRender()
{
    auto graphics = CreateScoped<Graphics>(m_Window);
    if (!graphics->IsInitialized()) return;

    m_Frequency = (double)SDL_GetPerformanceFrequency();

    auto ubo = CreateShared<UniformBuffer>();
    ubo->Define(BufferUsage::DYNAMIC, 
        sizeof(Matrix4) * 2     // Projection + View
        + sizeof(Vector3)       // Camera Position
        + sizeof(float) * 4);   // Coefficients for debuging

    auto program = graphics->CreateProgram("Basic", "", "");
    auto textProgram = graphics->CreateProgram("Text", "", "");

    auto cube = Cube(program);
    auto textRenderer = CreateScoped<TextRenderer>(textProgram, 3.0f);

    m_Camera = CreateShared<Camera>();
    m_Camera->SetPerspective(60.0f, 0.1f, 100.0f);
    m_Camera->SetPosition({0.0f, 0.5f, 3.0f});

    m_CameraController = CreateShared<SceneCameraController>();
    m_CameraController->Attach(m_Camera);

    // Used to be mark for mutliple render states.
    m_RenderState = true;

    // Static uniform data
    ubo->Bind(0);
    ubo->SetData(0, sizeof(Matrix4), m_Camera->GetProjection().Data());
    SDL_GL_MakeCurrent(SDL_GL_GetCurrentWindow(), SDL_GL_GetCurrentContext());
    while (m_RenderState & !m_Input->ShouldExit())
    {
        // Poll input events.
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
        }

        uint64_t currentTime = SDL_GetPerformanceCounter();
        m_DeltaTime = (currentTime - m_LastTime) / m_Frequency;
        m_LastTime = currentTime;
        m_FPS = 1.0 / m_DeltaTime;

        ubo->Bind(0);
        ubo->SetData(sizeof(Matrix4), sizeof(Matrix4), m_Camera->GetView().Data());
        /// ====================================================================
        graphics->Clear(BufferBitType::COLOR | BufferBitType::DEPTH);

        graphics->SetDepthTest(true);
        cube.Draw();

        graphics->SetDepthTest(false);
        textRenderer->Render({8}, FormatString("Phaten Engine\nFPS:%.2f", m_FPS));
        /// ====================================================================
        // Call window to swap buffers.
        graphics->Present();
    }
}

} // namespace Pt