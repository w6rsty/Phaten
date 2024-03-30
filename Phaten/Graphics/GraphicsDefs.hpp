#pragma once

#include <cstddef>
#include <string>

#include <glad/glad.h>

namespace Pt {

static const std::string PT_GL_VERSION = "410 core";
static const size_t MAX_UNIFORM_BUFFER_SLOTS = 16;

#ifdef __APPLE__
static const size_t MAX_TEXTURE_SLOTS = 16;
#else
static const size_t MAX_TEXTURE_SLOTS = 32;
#endif

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
    U_PROJECTION_VIEW,
    U_MODEL,
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

enum class TextureWrapMode
{
    REPEAT = 0,
    MIRRORED_REPEAT,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MAX_WRAP_MODE
};

enum class TextureFilterMode
{
    NEAREST = 0,
    LINEAR,
    MAX_FILTER_MODE
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

extern const GLenum TextureWrapModeGLType[];
extern const GLenum TextureFilterModeGLType[];

template <typename T>
constexpr size_t EnumAsIndex(T type)
{
    return static_cast<size_t>(type);
}

/// ===================================================

std::string AttributesBitToString(unsigned attributes);

/// Generate attribute information deubg string.
std::string ShowAttributes(unsigned attributes);

} // namespace Pt