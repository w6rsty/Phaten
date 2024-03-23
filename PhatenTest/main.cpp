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
    Pt::Window window{"Phaten", Pt::IntV2{800, 600}, Pt::ScreenMode::WINDOWED};

    float vertices[] = {
        -0.5f, -0.5f, 1.0f,
         0.5f, -0.5f, 1.0f,
         0.5f,  0.5f, 1.0f,
        -0.5f,  0.5f, 1.0f,
    };

    Pt::VertexBuffer vertexBuffer;

    Pt::VertexLayout layout = {
        {Pt::VertexElementType::FLOAT3, Pt::VertexElementSemantic::POSITION},
    };

    vertexBuffer.Define(Pt::BufferUsage::STATIC, 4, layout, vertices);
    vertexBuffer.Bind(vertexBuffer.Attributes());

    unsigned indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    Pt::IndexBuffer indexBuffer;
    indexBuffer.Define(Pt::BufferUsage::STATIC, 6, indices);
    indexBuffer.Bind();

    std::string shaderSource = ReadFile("Shaders/test.glsl");
    ShaderProgram shader(shaderSource, "test", std::string(), std::string());

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.Flush();
    }

    return 0;
}