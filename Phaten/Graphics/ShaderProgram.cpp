#include "ShaderProgram.hpp"

#include <iostream>

#include <glad/glad.h>

#include "Core/Logger.hpp"
#include "IO/StringUtils.hpp"

namespace Pt {

static ShaderProgram* boundProgram = nullptr;

ShaderProgram::ShaderProgram(
    std::string_view sourceCode,
    std::string_view shaderName,
    std::string_view vsDefines,
    std::string_view fsDefines
) :
    m_Handle(0),
    m_Attributes(0)
{
    // TODO: Ensure graphics system loaded.
    m_ShaderName = vsDefines.length() ? 
        StringViewConcatenate('_', shaderName, vsDefines, fsDefines) : 
        StringViewConcatenate('_', shaderName, fsDefines);

    Create(sourceCode, Split(vsDefines), Split(fsDefines));
}

ShaderProgram::~ShaderProgram()
{
    // TODO: Ensure graphics system loaded.
    Release();
}

bool ShaderProgram::Bind()
{
    if (!m_Handle)
    {
        PT_LOG_WARN("Shader program has not been created");
        return false;
    }

    if (boundProgram == this)
    {
        return true;
    }

    glUseProgram(m_Handle);
    boundProgram = this;
    return true;
}

int ShaderProgram::Uniform(std::string_view name) const
{
    return Uniform(StringHash(name));
}

int ShaderProgram::Uniform(StringHash name) const
{
    auto it = m_Uniforms.find(name);
    return it != m_Uniforms.end() ? it->second : -1;
}

int ShaderProgram::Uniform(PresetUniform name) const
{
    return m_PresetUniforms[static_cast<size_t>(name)];
}

bool ShaderProgram::Create(
    std::string_view sourceCode,
    const std::vector<std::string>& vsDefines,
    const std::vector<std::string>& fsDefines
)
{
    // Vertex shader ======================================
    std::string vsSourceCode;
    vsSourceCode += "#version 410 core\n";
    vsSourceCode += "#define COMPILE_VS\n";
    // for (const auto& define : vsDefines)
    // {
    //     vsSourceCode += "#define " + define + "\n";
    // }
    vsSourceCode += sourceCode;
    RemoveFunction(vsSourceCode, "void frag()");
    ReplaceIn(vsSourceCode, "vert()", "main()");
    const char* vsSourceStr = vsSourceCode.c_str();

    std::cout << vsSourceCode << std::endl;

    int vsCompiled;
    unsigned vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSourceStr, nullptr);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &vsCompiled);

    // Fragment shader ====================================
    std::string fsSourceCode;
    fsSourceCode += "#version 410 core\n";
    fsSourceCode += "#define COMPILE_FS\n";
    // for (const auto& define : fsDefines)
    // {
    //     fsSourceCode += "#define " + define + "\n";
    // }
    fsSourceCode += sourceCode;
    RemoveFunction(fsSourceCode, "void vert()");
    ReplaceIn(fsSourceCode, "frag()", "main()");
    const char* fsSourceStr = fsSourceCode.c_str();

    std::cout << fsSourceCode << std::endl;

    int fsCompiled;
    unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSourceStr, nullptr);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &fsCompiled);

    if (!vsCompiled || !vsCompiled)
    {
        glDeleteShader(vs);
        glDeleteShader(fs);
        return false;
    }

    m_Handle = glCreateProgram();
    glAttachShader(m_Handle, vs);
    glAttachShader(m_Handle, fs);
    glLinkProgram(m_Handle);
    glDeleteShader(vs);
    glDeleteShader(fs);

    int linked;
    glGetProgramiv(m_Handle, GL_LINK_STATUS, &linked);

    return true;
}

void ShaderProgram::Release()
{
    if (m_Handle)
    {
        glDeleteProgram(m_Handle);
        m_Handle = 0;

        if (boundProgram == this)
        {
            boundProgram = nullptr;
        }
    }
}

} // namespace Pt