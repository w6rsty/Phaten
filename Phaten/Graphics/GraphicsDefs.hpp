#pragma once

#include <cstddef>
#include <string>

namespace Pt {

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

enum class PresetUniform : size_t
{
    U_WORLD_MATRIX,
    MAX_PRESET_UNIFORMS
};


extern const unsigned VertexElementGLCount[];
extern const unsigned VertexElementGLType[];
extern const unsigned VertexElementSize[];
extern const unsigned VertexAttributeIdx[];

std::string AttributesToString(unsigned attributes);

} // namespace Pt