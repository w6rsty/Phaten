#pragma once

#include <map>

#include "Object/Ptr.hpp"
#include "Input/Window.hpp"
#include "Math/IntVector.hpp"
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
    ~Graphics();

    void SetVSync(bool enable);

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

    IntV2 Size() const;
    int Width() const;
    int Height() const;

    void Present();
private:
    bool m_VSync;

    ScopedPtr<Window> m_Window;
    ScopedPtr<GraphicsContext> m_GraphicsContext;

    std::map<StringHash, SharedPtr<Shader>> m_Shaders;
};

} // namespace Pt