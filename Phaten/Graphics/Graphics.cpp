#include "Graphics.hpp"

#include <glad/glad.h>

#include "IO/Assert.hpp"
#include "ShaderProgram.hpp"

namespace Pt {

Graphics::Graphics(const SharedPtr<Window>& window) :
    m_Window(window),
    m_VSync(true)
{
    PT_TAG_INFO("Graphics", "Created graphics system");
    // Use the window handle to create the graphics context.
    m_GraphicsContext = CreateScoped<GraphicsContext>(m_Window->SDLHandle());

    // Create a default VAO and use forever :D
    unsigned defaultVAO;
    glGenVertexArrays(1, &defaultVAO);
    glBindVertexArray(defaultVAO);

    // Initialization Done ====================================================
    glEnable(GL_DEPTH_TEST);
}

Graphics::~Graphics()
{
    PT_TAG_INFO("Graphics", "Exited graphics system");
}

void Graphics::SetVSync(bool enable)
{
    m_Window->SetVSync(enable);
    m_VSync = enable;
}

void Graphics::SetClearColor(const Vector4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
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

void Graphics::SetUniform(ShaderProgram* program, PresetUniform uniform, const Matrix4& value)
{
    if (program)
    {
        int location = program->Uniform(uniform);
        if (location >= 0)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, value.Data());
        }
    }
}

void Graphics::SetFrameBuffer(FrameBuffer* frameBuffer)
{
    if (frameBuffer)
    {
        frameBuffer->Bind();
    }
    else
    {
        FrameBuffer::Unbind();
    }
}

IntV2 Graphics::Size() const
{
    return m_Window->Size();
}

void* Graphics::GetNativeWindow() const
{
    return m_Window->SDLHandle();
}


void Graphics::Draw(PrimitiveType type, size_t first, size_t count)
{
    glDrawArrays(PrimitiveGLType[EnumAsIndex(type)], first, count);
}
void Graphics::DrawIndexed(PrimitiveType type, size_t first, size_t count)
{   
    glDrawElements(
        PrimitiveGLType[EnumAsIndex(type)],
        count,
        GL_UNSIGNED_INT,
        (void*)(first * sizeof(unsigned))
    );
}

void Graphics::Present()
{
    m_Window->Swap();
}

void Graphics::Clear()
{
    /// TODO: use bitmask to select which buffer to clear.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace Pt