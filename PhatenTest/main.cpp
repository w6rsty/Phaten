#include <glad/glad.h>
#include <SDL.h>

#include "Core/Logger.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Input/Window.hpp"
#include "Graphics/Graphics.hpp"
#include "Math/Vector.hpp"
#include "Thread/ThreadUtils.hpp"

using namespace Pt;

int main(int argc, char *argv[])
{
    PT_LOG_INFO("Hardware concurrency: ", CPUCount());
    
    Graphics graphics {
        {"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED}
    };

    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
    };

    VertexBuffer vertexBuffer;

    VertexLayout layout = {
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
        {VertexElementType::FLOAT2, VertexElementSemantic::TEXCOORD},
    };

    vertexBuffer.Define(BufferUsage::STATIC, 4, layout, vertices);
    vertexBuffer.Bind(vertexBuffer.Attributes());

    unsigned indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    IndexBuffer indexBuffer;
    indexBuffer.Define(BufferUsage::STATIC, 6, indices);
    indexBuffer.Bind();
    
    Shader shader;
    shader.Define("Phaten/Shaders/Test.glsl");
    auto program = SharedPtr(shader.CreateProgram("Test", "", ""));

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        program->Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer.NumIndices(), GL_UNSIGNED_INT, 0);

        graphics.Present();
    }

    return 0;
}