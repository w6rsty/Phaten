#pragma once

#include <map>

#include "Core/Ptr.hpp"
#include "Input/Window.hpp"
#include "Math/Vector.hpp"
#include "IO/StringHash.hpp"
#include "GraphicsContext.hpp"
#include "Shader.hpp"

struct SDL_Window;

namespace Pt {

class ShaderProgram;

/// Graphics API interface.
class Graphics
{
public:
    Graphics(WindowCreateInfo windowInfo);
    /// Load a shader from file. Or return the existing one.
    SharedPtr<Shader> LoadShader(std::string_view name);
    /// Create a shader program from the shader soure code.
    SharedPtr<ShaderProgram> CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, float value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector2& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector3& value);
    void SetUniform(ShaderProgram* program, PresetUniform uniform, const Vector4& value);

    void SetupCanvas();
    void RenderCanvas();

    void Present();
private:
    ScopedPtr<Window> m_Window;
    ScopedPtr<GraphicsContext> m_GraphicsContext;

    std::map<StringHash, SharedPtr<Shader>> m_Shaders;
};

} // namespace Pt