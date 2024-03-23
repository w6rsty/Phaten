#include "ShaderProgram.hpp"

#include <utility>

#include <glad/glad.h>

#include "Core/Core.hpp"
#include "Core/Assert.hpp"
#include "IO/StringUtils.hpp"

namespace Pt {

static ShaderProgram* boundProgram = nullptr;
/// Max buffer length for shader attribute name querying
static const size_t MAX_NAME_LENGTH = 256;

static int NumberPostFix(std::string_view str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        if (isdigit(str[i]))
        {
            return ParseInt(str.substr(i));
        }
    }
    return -1;
}

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
        // Prevent too many error messages.
        PT_ASSERT_MSG(false, "Shader program has not been created");
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

void ShaderProgram::Create(
    std::string_view sourceCode,
    const std::vector<std::string>& vsDefines,
    const std::vector<std::string>& fsDefines
)
{
    // Compile shaders
    auto[vs, vsCompiled] = CreateShader(ShaderType::Vertex, sourceCode, vsDefines);
    auto[fs, fsCompiled] = CreateShader(ShaderType::Fragment, sourceCode, fsDefines);

    // Validation
    if (!vsCompiled || !fsCompiled)
    {
        glDeleteShader(vs);
        glDeleteShader(fs);
        return;
    }

    // Create program and link
    m_Handle = glCreateProgram();
    glAttachShader(m_Handle, vs);
    glAttachShader(m_Handle, fs);
    glLinkProgram(m_Handle);
    glDeleteShader(vs);
    glDeleteShader(fs);

    int linked;
    glGetProgramiv(m_Handle, GL_LINK_STATUS, &linked);

    {
        if (linked == GL_FALSE)
        {
            std::string errorString;
            GLint errorLength = 0;
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &errorLength);
            errorString.resize(errorLength);
            glGetProgramInfoLog(m_Handle, errorLength, &errorLength, errorString.data());
            PT_TAG_WARN("Shader", errorString);
        }
    }

    // Get used attributes ========================================  

    // Buffer for queried name.
    char nameBuffer[MAX_NAME_LENGTH];
    // Number of attributes.
    int numAttrbutes;
    // Number of uniforms.
    int numUniforms;
    // Number of uniform blocks.
    int numUniformBlocks;
    // Attribute name length.
    int nameLength;
    //  Size.
    int sizeElement;
    // GL type
    GLenum queryGLType;

    m_Attributes = 0; // Reset attributes.
    // Querying number of used attributes.
    glGetProgramiv(m_Handle, GL_ACTIVE_ATTRIBUTES, &numAttrbutes);

    for (int idx = 0; idx < numAttrbutes; ++idx)
    {
        glGetActiveAttrib(
            m_Handle,
            idx,
            static_cast<GLsizei>(MAX_NAME_LENGTH),
            &nameLength,
            &sizeElement,
            &queryGLType,
            nameBuffer
        );

        std::string attributeName(nameBuffer, nameLength);
        size_t attributeIdx = IndexOfList(attributeName, VertexAttributeName, 0x7F);
        if (m_Attributes < 32)
        {
            m_Attributes |= (1 << attributeIdx);
        }
    }

    // Get used uniforms ============================================

    m_Uniforms.clear(); // Reset uniforms

    int status = Bind(); 

    glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &numUniforms);
    // Reset preset uniforms
    for (int& presetUniform : m_PresetUniforms)
    {
        presetUniform = -1;
    }

    for (int idx = 0; idx < numUniforms; ++idx)
    {
        glGetActiveUniform(
            m_Handle,
            idx,
            static_cast<GLsizei>(MAX_NAME_LENGTH),
            &nameLength,
            &sizeElement,
            &queryGLType, 
            nameBuffer
        );

        std::string uniformName(nameBuffer, nameLength);
        // Remove [0] if uniform is an array.(not neccessery but recommend doing this)
        ReplaceIn(uniformName, "[0]", ""); 
        int location = glGetUniformLocation(m_Handle, uniformName.c_str());
        m_Uniforms[StringHash(uniformName)] = location;

        size_t preset = IndexOfList(uniformName, PresetUniformName, MAX_NAME_LENGTH);
        if (preset < static_cast<size_t>(PresetUniform::MAX_PRESET_UNIFORMS))
        {
            m_PresetUniforms[preset] = location;
        }

        // Set sampler uniforms
        if ((queryGLType >= GL_SAMPLER_1D && queryGLType <= GL_SAMPLER_2D_SHADOW) ||
            (queryGLType >= GL_SAMPLER_1D_ARRAY && queryGLType <= GL_SAMPLER_CUBE_SHADOW) ||
            (queryGLType >= GL_INT_SAMPLER_1D && queryGLType <= GL_UNSIGNED_INT_SAMPLER_2D_ARRAY))
        {
            int unit = NumberPostFix(uniformName);

            if (unit < 0)
            {
                continue;
            }

            if (sizeElement > 1)
            {
                std::vector<int> units;
                for (int i = 0; i < sizeElement; ++i)
                {
                    units.push_back(unit + i);
                }
                glUniform1iv(location, sizeElement, units.data());
            }
            else
            {
                glUniform1i(location, unit);
            }
        }        
    }

    glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);
    for (int idx = 0; idx < numUniformBlocks; ++idx)
    {
        glGetActiveUniformBlockName(
            m_Handle, 
            idx, 
            static_cast<GLsizei>(MAX_NAME_LENGTH), 
            &nameLength, 
            nameBuffer
        );

        std::string uniformBlockName(nameBuffer, nameLength);
        int uniformBlockIdx = glGetUniformBlockIndex(m_Handle, uniformBlockName.c_str());
        int bindingIdx = NumberPostFix(uniformBlockName);
        if (bindingIdx < 0)
        {
            bindingIdx = uniformBlockIdx;
        }

        glUniformBlockBinding(m_Handle, uniformBlockIdx, bindingIdx);
    }

    PT_LOG_INFO("Created linked shader program: ", m_ShaderName);
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

std::pair<unsigned, int> ShaderProgram::CreateShader(
    ShaderType type,
    std::string_view sourceCode,
    const std::vector<std::string>& defines
)
{
    std::string shaderSourceCode;
    shaderSourceCode += "#version " + PT_GL_VERSION + "\n";
    shaderSourceCode += (type == ShaderType::Vertex) ? "#define COMPILE_VS\n" : "#define COMPILE_FS\n";
    for (const auto& define : defines)
    {
        shaderSourceCode += "#define " + define + "\n";
    }
    shaderSourceCode += std::string(sourceCode);
    CommentFunction(shaderSourceCode, (type == ShaderType::Vertex) ? "void frag()" : "void vert()");
    ReplaceIn(shaderSourceCode, (type == ShaderType::Vertex) ? "vert()" : "frag()", "main()");
    const char* shaderSourceStr = shaderSourceCode.c_str();

// print shader source code
#ifdef PT_SHADER_DEBUG_SHOW
    PrintByLine(shaderSourceStr);
#endif

    int shaderCompiled;
    unsigned shader = glCreateShader((type == ShaderType::Vertex) ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &shaderSourceStr, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

    if (shaderCompiled == GL_FALSE)
    {
        std::string errorString;
        GLint errorLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLength);
        errorString.resize(errorLength);
        glGetShaderInfoLog(shader, errorLength, &errorLength, errorString.data());
        PT_TAG_WARN("Shader", (
            type == ShaderType::Vertex) ? "Vertex" : "Fragment", " shader compilation failed:\n", errorString);

// Print error shader source code
#ifdef PT_SHADER_DEBUG
        PrintShaderByLine(shaderSourceStr, errorString);        
#endif
    }

    return {shader, shaderCompiled};
}

} // namespace Pt