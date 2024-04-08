#include "Application.hpp"

#include <SDL.h>

#include "Graphics/GraphicsDefs.hpp"
#include "Graphics/Texture.hpp"
#include "IO/StringUtils.hpp"

#include "Graphics/UniformBuffer.hpp"
#include "Graphics/Texture.hpp"

#include "Object/Ptr.hpp"
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

    SharedPtr<Texture> colorAttachment = Object::FactoryCreate<Texture>();
    colorAttachment->Define(TextureType::TEX_2D, sWindowSize * 2, ImageFormat::RGBA8, nullptr);

    SharedPtr<Texture> depthAttachment = Object::FactoryCreate<Texture>();
    depthAttachment->Define(TextureType::TEX_2D, sWindowSize * 2, ImageFormat::D24S8, nullptr);

    auto fbo = CreateShared<FrameBuffer>();
    fbo->Define(colorAttachment, depthAttachment);

    auto ubo = CreateShared<UniformBuffer>();
    ubo->Define(BufferUsage::DYNAMIC, 
        sizeof(Matrix4) * 2     // Projection + View
        + sizeof(Vector3)       // Camera Position
        + sizeof(float) * 4);   // Coefficients for debuging

    auto program = graphics->CreateProgram("Basic", "", "");
    auto textProgram = graphics->CreateProgram("Text", "", "");
    auto postProgram = graphics->CreateProgram("Post", "", "ENABLE");

    auto demoCube = Cube(program);
    auto screen = ScreenPlane(postProgram);
    auto textRenderer = CreateScoped<TextRenderer>(textProgram, 3.0f);

    // for demo
    SharedPtr<Image> demoImage = Object::FactoryCreate<Image>();
    demoImage->Load("Assets/Textures/player.png");

    SharedPtr<Texture> demoTexture = Object::FactoryCreate<Texture>();
    demoTexture->Define(TextureType::TEX_2D, demoImage);
    demoTexture->SetFilterMode(TextureFilterMode::NEAREST);

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
    bool showDebug = false;
    std::string debugString = "Phaten Engine\nFPS:%.2f\nVSync:%s\nCamera Rotation:%s\nCamera Position:%s";
    while (m_RenderState & !m_Input->ShouldExit())
    {
        // Poll input events.
        {        
            m_Input->Update();
            
            if (m_Input->KeyPressed(SDLK_ESCAPE))
                m_RenderState = false;

            if (m_Input->KeyPressed(SDLK_F1))
                Graphics::SetWireframe(!Graphics::IsWireframe());
            if (m_Input->KeyPressed(SDLK_F2))
                graphics->SetVSync(!graphics->IsVSync());
            if (m_Input->KeyPressed(SDLK_F3))
                showDebug = !showDebug;

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
        Graphics::SetFrameBuffer(fbo);
        Graphics::Clear(BufferBitType::COLOR | BufferBitType::DEPTH);

        demoTexture->Bind(1);
        Graphics::SetDepthTest(true);
        demoCube.Draw();
        /// ====================================================================
        if (showDebug)
        {
            Graphics::Clear(BufferBitType::DEPTH);
            Graphics::SetDepthTest(true);
            textRenderer->Render({8}, 
                FormatString(debugString.c_str(), 
                    m_FPS, 
                    graphics->IsVSync() ? "On" : "Off",
                    m_Camera->GetRotation().ToString().c_str(),
                    m_Camera->GetPosition().ToString().c_str()
                )
            );
        }
        /// ====================================================================
        Graphics::SetFrameBuffer(nullptr);
        Graphics::Clear(BufferBitType::COLOR);

        colorAttachment->Bind(2);
        Graphics::SetDepthTest(false);
        screen.Draw();
        /// ====================================================================
        // Call window to swap buffers.
        graphics->Present();
    }
}

} // namespace Pt