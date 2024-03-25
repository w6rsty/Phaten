#include "Graphics.hpp"

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "IO/Assert.hpp"
#include "ShaderProgram.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Pt {

struct CanvasData
{
    SharedPtr<VertexBuffer> vertexBuffer;
    SharedPtr<IndexBuffer> indexBuffer;
    SharedPtr<ShaderProgram> program;

    static constexpr float vertices[] = {
        -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
         1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,
    };
    
    static constexpr unsigned indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
};

static CanvasData* s_CanvasData = nullptr;

Graphics::Graphics(WindowCreateInfo windowInfo)
{
    PT_TAG_INFO("Graphics", "Created graphics system");

    // TODO: Add another struct to store window properties.
    m_Window = CreateScoped<Window>(windowInfo.title, windowInfo.windowSize, windowInfo.mode);
    // Use the window handle to create the graphics context.
    m_GraphicsContext = CreateScoped<GraphicsContext>(m_Window->NativeHandle());

    // Create a default VAO and use forever :D
    // (I'd like to switch to vulkan soon)
    unsigned defaultVAO;
    glGenVertexArrays(1, &defaultVAO);
    glBindVertexArray(defaultVAO);

    // Initialization Done ====================================================
    
    if (!s_CanvasData)
    {
        s_CanvasData = new CanvasData{};
    }
}

Graphics::~Graphics()
{
    if (s_CanvasData)
    {
        delete s_CanvasData;
    }

    PT_TAG_INFO("Graphics", "Exited graphics system");
}

void Graphics::SetVSync(bool enable)
{
    SDL_GL_SetSwapInterval(enable ? 1 : 0);
    m_VSync = enable;
}

SharedPtr<Shader> Graphics::LoadShader(std::string_view name)
{
    auto hash = StringHash(name);
    auto it = m_Shaders.find(hash);
    if (it != m_Shaders.end())
    {
        return it->second;
    }
    auto shader = CreateShared<Shader>();
    std::string path = "Phaten/Shaders/" + std::string(name) + ".glsl";
    shader->Define(path);
    m_Shaders[hash] = shader;
    return shader;
}

SharedPtr<ShaderProgram> Graphics::CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines)
{
    auto shader = LoadShader(name);
    return shader->CreateProgram(name, vsDefines, fsDefines);
}

void Graphics::SetUniform(ShaderProgram* program, PresetUniform uniform, float value)
{
    if (program)
    {
        int location = program->Uniform(uniform);
        if (location >= 0)
        {
            glUniform1f(location, value);
        }
    }
}

void Graphics::SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector2& value)
{
    if (program)
    {
        int location = program->Uniform(uniform);
        if (location >= 0)
        {
            glUniform2f(location, value.x, value.y);
        }
    }
}

void Graphics::SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector3& value)
{
    if (program)
    {
        int location = program->Uniform(uniform);
        if (location >= 0)
        {
            glUniform3f(location, value.x, value.y, value.z);
        }
    }
}

void Graphics::SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector4& value)
{
    if (program)
    {
        int location = program->Uniform(uniform);
        if (location >= 0)
        {
            glUniform4f(location, value.x, value.y, value.z, value.w);
        }
    }
}

void Graphics::SetupCanvas()
{
    s_CanvasData->vertexBuffer = CreateShared<VertexBuffer>();
    s_CanvasData->vertexBuffer->Define(BufferUsage::STATIC, 4, VertexLayout{
        {VertexElementType::FLOAT3, VertexElementSemantic::POSITION},
    }, s_CanvasData->vertices);
    s_CanvasData->vertexBuffer->Bind(s_CanvasData->vertexBuffer->Attributes());

    s_CanvasData->indexBuffer = CreateShared<IndexBuffer>();
    s_CanvasData->indexBuffer->Define(BufferUsage::STATIC, 6, s_CanvasData->indices);
    s_CanvasData->indexBuffer->Bind();

    LoadShader("Basic");
    s_CanvasData->program = CreateProgram("Basic", "", "");
}

void Graphics::RenderCanvas()
{
    auto& program = s_CanvasData->program;
    glClear(GL_COLOR_BUFFER_BIT);
    program->Bind();
    float time = SDL_GetTicks() / 1000.0f;
    SetUniform(program.Get(), PresetUniform::U_TIME, time);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

IntV2 Graphics::Size() const
{
    IntV2 size;
    SDL_GetWindowSize(m_Window->NativeHandle(), (int*)&size.x, (int*)&size.y);
    return size;
}

int Graphics::Width() const
{
    return Size().x;
}

int Graphics::Height() const
{
    return Size().y;
}

void Graphics::Present()
{
    SDL_GL_SwapWindow(m_GraphicsContext->m_WindowHandle);
}

} // namespace Pt