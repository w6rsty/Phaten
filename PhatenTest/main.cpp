#include <glad/glad.h>
#include <SDL.h>

#include "Graphics/Shader.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Input/Window.hpp"
#include "Math/Vector.hpp"

using namespace Pt;

int main(int argc, char *argv[])
{
    Window window{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED};

    float vertices[] = {
        -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
         1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,
    };

    VertexBuffer vertexBuffer;

    VertexLayout layout = {
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
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
    
    Pt::Vector4 color {1, 0, 1, 1};

    UniformBuffer uniformBuffer;
    uniformBuffer.Define(BufferUsage::STATIC, sizeof(float) * 4, &color);
    uniformBuffer.Bind(0);

    Shader shader;
    shader.Define("Shaders/Test.glsl");
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

        window.Flush();
    }

    return 0;
}