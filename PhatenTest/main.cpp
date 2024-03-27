#include <SDL.h>

#include "Graphics/GraphicsDefs.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/Vertex.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "IO/Logger.hpp"
#include "Graphics/Graphics.hpp"
#include "Math/Matrix.hpp"
#include "Math/Space.hpp"
#include "Math/Vector.hpp"
#include "Thread/ThreadUtils.hpp"

#include "Math/Transform.hpp"

using namespace Pt;

int main(int argc, char *argv[])
{
    PT_LOG_INFO("Hardware concurrency: ", CPUCount());
    
    auto graphics = CreateScoped<Graphics>(WindowCreateInfo{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED});
    
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    VertexBuffer vb;
    vb.Define(BufferUsage::STATIC, 4, {{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
    }}, vertices);
    vb.Bind(vb.Attributes());

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    IndexBuffer ib;
    ib.Define(BufferUsage::STATIC, 6, indices);
    ib.Bind();

    graphics->LoadShader("Basic");
    auto program = graphics->CreateProgram("Basic", "", "");

    Matrix4 projection = Perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    Matrix4 view = LookAt(Vector3{0.0f, 0.0f, 3.0f}, Vector3{0.0f, 0.0f, 0.0f}, Vector3{0.0f, 1.0f, 0.0f});

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        program->Bind();
        graphics->SetUniform(program, PresetUniform::U_WORLD_MATRIX, projection * view);

        graphics->DrawIndexed(PrimitiveType::TRIANGLES, 0, 6);
        graphics->Present();
    }

    return 0;
}