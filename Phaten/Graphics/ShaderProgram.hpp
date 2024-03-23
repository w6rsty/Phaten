#pragma once

#include <map>
#include <vector>
#include <string>
#include <string_view>

#include "Core/Ptr.hpp"
#include "IO/StringHash.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

/*  
Shader Program receive processed shader source.
Perform some post processing. Then compile and link to generate a
OpenGL shader program.
Most in:
ShaderProgram::Create()
1. Handle macros
    - Add macro definition for vertex and fragment shader.
2. Compile shaders
3. Link shader program
4. Collect shader infos
    - Get used attributes and convert into a bitmask.
    - Get used uniform.
    - Get used uniform blokcks, bind them to related binding point.
*/

/// Linked shader program.
class ShaderProgram : public RefCounted
{
public:
    ShaderProgram(
        std::string_view sourceCode,
        std::string_view shaderName,
        std::string_view vsDefines,
        std::string_view fsDefines
    );
    ~ShaderProgram();

    /// Bind shader program
    bool Bind();

    std::string_view ShaderName() const { return m_ShaderName; } 
    unsigned Attributes() const { return m_Attributes; }
    const std::map<StringHash, int>& Uniforms() const { return m_Uniforms; }

    /// Get uniform location by name. Negative means not found.
    int Uniform(std::string_view name) const;
    /// Get uniform location by string hash. Negative means not found.
    int Uniform(StringHash name) const;
    /// Get preset unfiorm location.
    int Uniform(PresetUniform name) const;

    unsigned GLHandle() const { return m_Handle; }

    static std::pair<unsigned, int> CreateShader(
        ShaderType type,
        std::string_view sourceCode,
        const std::vector<std::string>& defines
    );
private:
    /// Compile and linked. Core function.
    void Create(
        std::string_view sourceCode,
        const std::vector<std::string>& vsDefines, 
        const std::vector<std::string>& fsDefines
    );
    void Release();

    /// OpenGL object identifier.
    unsigned m_Handle;
    /// Used vertex attributes bitmask.
    unsigned m_Attributes;
    /// Store all uniform locations.
    std::map<StringHash, int> m_Uniforms;
    /// Store preset uniform locations.
    int m_PresetUniforms[static_cast<size_t>(PresetUniform::MAX_PRESET_UNIFORMS)];
    /// Shader name.
    std::string m_ShaderName;
};

} // namespace Pt