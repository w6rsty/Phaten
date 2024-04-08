#pragma once

#include <map>
#include <utility>

#include "Object/Ptr.hpp"
#include "Object/Object.hpp"
#include "IO/StringHash.hpp"
#include "ShaderProgram.hpp"
#include "GraphicsDefs.hpp"

namespace Pt {

/// Shader soure code class that manages shader programs variations.
class Shader : public Object
{
    OBJECT(Shader)
public:
    Shader();
    ~Shader();

    static void RegisterObject();

    void Define(std::string_view path);

    ShaderProgram* CreateProgram(std::string_view name, std::string_view vsDefines, std::string_view fsDefines);

    std::string_view SourceCode() const { return m_SourceCode; }
private:
    bool ProcessInclude(std::string& code);

    /// Vertex and fragment pair as key.
    using ShaderProgramKey = std::pair<StringHash, StringHash>;
    std::map<ShaderProgramKey, SharedPtr<ShaderProgram>> m_Programs;

    std::string m_SourceCode;
};

} // namespace Pt