#pragma once

#include <map>

#include "Object/Ptr.hpp"
#include "IO/StringHash.hpp"
#include "Input/Window.hpp"
#include "Math/IntVector.hpp"
#include "Math/Matrix.hpp"
#include "Shader.hpp"
#include "GraphicsContext.hpp"
#include "FrameBuffer.hpp"

struct SDL_Window;

namespace Pt {

class ShaderProgram;

/// Graphics API interface.
class Graphics
{
public:
    Graphics(const SharedPtr<Window>& window);
    ~Graphics();

    void SetVSync(bool enable);
    void SetClearColor(const Vector4& color = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    void SetDepthTest(bool enable);

    /// Load a shader from file. Or return the existing one.
    SharedPtr<Shader> LoadShader(std::string_view name);
    /// Create a shader program from the shader soure code.
    SharedPtr<ShaderProgram> CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, int value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, int* values, size_t count);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, float value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector2& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector3& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector4& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Matrix4& value);

    void SetUniform(ShaderProgram* program, std::string_view name, int value);
    void SetUniform(ShaderProgram* program, std::string_view name, int* values, size_t count);
    void SetUniform(ShaderProgram* program, std::string_view name, float value);
    void SetUniform(ShaderProgram* program, std::string_view name, const Vector2& value);
    void SetUniform(ShaderProgram* program, std::string_view name, const Vector3& value);
    void SetUniform(ShaderProgram* program, std::string_view name, const Vector4& value);

    void SetFrameBuffer(FrameBuffer* frameBuffer);

    bool IsVSync() const { return m_VSync; }

    static void Draw(PrimitiveType type, size_t first, size_t count);
    static void DrawIndexed(PrimitiveType type, size_t first, size_t count);

    IntV2 Size() const;
    void* GetNativeWindow() const;
    WeakPtr<Window> GetWindow() const { return m_Window; }

    void Present();
    static void Clear(unsigned bits = 1);
private:
    bool m_VSync;
    bool m_DepthTest;

    SharedPtr<Window> m_Window;
    ScopedPtr<GraphicsContext> m_GraphicsContext;

    std::map<StringHash, SharedPtr<Shader>> m_Shaders;
};

} // namespace Pt