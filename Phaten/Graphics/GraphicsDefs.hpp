#pragma once

#include <cstddef>
#include <string>

namespace Pt {

static const unsigned NO_GL_HANDLE = -1;

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
enum class VertexElementType
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
enum class VertexElementSemantic
{
    POSITION = 0,
    NORMAL,
    TANGENT,
    VERTEX_COLOR,
    TEXCOORD,
    MAX_ELEMENT_SEMANTIC
};

enum class VertexAttributeType
{
    POSTION = 0,
    NORMAL,
    TANGENT,
    VERTEX_COLOR,
    TEXCOORD,
    MAX_ATTRIBUTE
};

extern const unsigned VertexElementGLCount[];
extern const unsigned VertexElementGLType[];
extern const unsigned VertexElementSize[];
extern const unsigned VertexAttributeIdx[];

std::string AttributesToString(unsigned attributes);

} // namespace Pt