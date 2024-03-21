#include <iostream>

#include <glad/glad.h>
#include <SDL.h>

#include "Graphics/VertexBuffer.hpp"
#include "Graphics/IndexBuffer.hpp"
#include "Input/Window.hpp"

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

    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;

        out vec3 vPos;

        void main()
        {
            gl_Position = vec4(aPos, 1.0);
            vPos = aPos;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        layout (location = 0) out vec4 FragColor;

        in vec3 vPos;

        void main()
        {
            vec3 color = vec3(vPos * 0.5 + 0.5);
            FragColor = vec4(color, 1.0);
        }
    )";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.Flush();
    }
    
    return 0;
}