#include "Shader.hpp"

#include "Core/Assert.hpp"
#include "IO/StringUtils.hpp"

namespace Pt {

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::Define(std::string_view path)
{
    m_SourceCode = ReadFile(path);
    bool status = ProcessInclude(m_SourceCode);
    PT_ASSERT(status);
}

ShaderProgram* Shader::CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines)
{
    ShaderProgramKey key = std::make_pair(StringHash(vsDefines), StringHash(fsDefines));

    // Check if the program already exists.
    auto it = m_Programs.find(key);
    if (it != m_Programs.end())
    {
        return it->second;
    }

    ShaderProgram* newVariation = new ShaderProgram(m_SourceCode, name, vsDefines, fsDefines);
    m_Programs[key] = newVariation;
    return newVariation;
}

bool Shader::ProcessInclude(std::string& code)
{
    size_t pos = 0;
    while ((pos = code.find("#include", pos)) != std::string::npos)
    {
        size_t start = code.find("\"", pos);
        size_t end = code.find("\"", start + 1);
        if (start == std::string::npos || end == std::string::npos)
        {
            PT_TAG_ERROR("Shader preprocess", "Invalid include directive");
            return false;
        }

        std::string includePath = code.substr(start + 1, end - start - 1);
        std::string includeCode = ReadFile(includePath);
        if (includeCode.empty())
        {
            PT_TAG_ERROR("Shader preprocess", "Failed to read include file");
            return false;
        }

        code.replace(pos, end - pos + 1, includeCode);
    }

    return true;
}

} // namespace Pt