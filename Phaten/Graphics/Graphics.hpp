#pragma once

#include <map>

#include "Object/Ptr.hpp"
#include "IO/StringHash.hpp"
#include "Input/Window.hpp"
#include "Math/IntVector.hpp"
#include "Math/Matrix.hpp"
#include "Shader.hpp"
#include "GraphicsContext.hpp"

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

    /// Load a shader from file. Or return the existing one.
    SharedPtr<Shader> LoadShader(std::string_view name);
    /// Create a shader program from the shader soure code.
    SharedPtr<ShaderProgram> CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, float value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector2& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector3& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector4& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Matrix4& value);

    static void Draw(PrimitiveType type, size_t first, size_t count);
    static void DrawIndexed(PrimitiveType type, size_t first, size_t count);

    IntV2 Size() const;
    void* GetNativeWindow() const;
    WeakPtr<Window> GetWindow() const { return m_Window; }

    void Present();
    static void Clear();
private:
    bool m_VSync;

    SharedPtr<Window> m_Window;
    ScopedPtr<GraphicsContext> m_GraphicsContext;

    std::map<StringHash, SharedPtr<Shader>> m_Shaders;
};

} // namespace Pt