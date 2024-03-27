#pragma once

#include <cstddef>
#include <string>

#include <glad/glad.h>

namespace Pt {

static const std::string PT_GL_VERSION = "410 core";
static const size_t MAX_UNIFORM_BUFFER_SLOTS = 16;

enum class ScreenMode
{
    WINDOWED = 0,
    FULLSCREEN,
    BORDERLESS_FULLSCREEN
};

enum class BufferUsage
{
    STATIC = 0,
    DYNAMIC,
};

/// element type for vertex element
enum class VertexElementType : size_t
{
    INT = 0,
    FLOAT,
    FLOAT2, 
    FLOAT3, 
    FLOAT4,
    UBTYE,
    MAX_ELEMENT_TYPE
};

/// Vertex attribute semantic
enum class VertexElementSemantic : size_t
{
    POSITION = 0,
    NORMAL,
    TANGENT,
    VERTEX_COLOR,
    TEXCOORD,
    MAX_ELEMENT_SEMANTIC
};

enum class VertexAttributeType : size_t
{
    POSTION = 0,
    NORMAL,
    TANGENT,
    VERTEX_COLOR,
    TEXCOORD,
    MAX_ATTRIBUTE
};

/// Don't forget to update PresetUniformName[] in 
/// GraphicsDefs.cpp when adding new preset uniform.
enum class PresetUniform : size_t
{
    U_TIME,
    U_WORLD_MATRIX,
    MAX_PRESET_UNIFORMS
};

enum class ShaderType
{
    Vertex,
    Fragment
};

enum class PrimitiveType
{
    POINTS = 0,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    MAX_PRIMITIVE_TYPE
};

extern const unsigned VertexElementGLCount[];
extern const unsigned VertexElementGLType[];
extern const unsigned VertexElementSize[];
extern const unsigned VertexAttributeIdx[];

extern const std::string VertexAttributeName[];
extern const std::string VertexElementSemanticName[];
extern const std::string PresetUniformName[];

extern const std::string PrimitiveName[];
extern const GLenum PrimitiveGLType[];

std::string AttributesBitToString(unsigned attributes);

/// Generate attribute information deubg string.
std::string ShowAttributes(unsigned attributes);

} // namespace Pt