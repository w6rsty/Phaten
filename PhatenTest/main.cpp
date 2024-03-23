#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <SDL.h>

#include "Graphics/GraphicsDefs.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Graphics/UniformBuffer.hpp"
#include "Graphics/ShaderProgram.hpp"
#include "Input/Window.hpp"
#include "Math/Vector.hpp"

using namespace Pt;

std::string ReadFile(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::in | std::ios::ate);
    if (!file.is_open())
    {
        return "";
    }
    std::string content;
    size_t size = file.tellg();
    content.resize(size);
    file.seekg(0, std::ios::beg);
    file.read(&content[0], size);
    file.close();
    return content;
}

int main(int argc, char *argv[])
{
    Window window{"Phaten", IntV2{800, 600}, ScreenMode::WINDOWED};

    float vertices[] = {
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    VertexBuffer vertexBuffer;

    VertexLayout layout = {
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
        {VertexElementType::FLOAT3, VertexElementSemantic::VERTEX_COLOR},
    };

    vertexBuffer.Define(BufferUsage::STATIC, 3, layout, vertices);
    vertexBuffer.Bind(vertexBuffer.Attributes());

    unsigned indices[] = {
        0, 1, 2,
    };

    IndexBuffer indexBuffer;
    indexBuffer.Define(BufferUsage::STATIC, 3, indices);
    indexBuffer.Bind();
    
    Pt::Vector4 color {1, 0, 1, 1};

    UniformBuffer uniformBuffer;
    uniformBuffer.Define(BufferUsage::STATIC, sizeof(float) * 4, &color);
    uniformBuffer.Bind(0);

    std::string shaderSource = ReadFile("Shaders/test.glsl");
    ShaderProgram shader(shaderSource, "Test", "", "");

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        shader.Bind();
        glDrawElements(GL_TRIANGLES, indexBuffer.NumIndices(), GL_UNSIGNED_INT, 0);

        window.Flush();
    }

    return 0;
}