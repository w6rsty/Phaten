#include "GraphicsDefs.hpp"

#include <bitset>

#include <glad/glad.h>

namespace Pt {

const unsigned VertexElementGLCount[]
{
    1,
    1,
    2,
    3,
    4,
    1
};

const unsigned VertexElementGLType[]
{
    GL_INT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_UNSIGNED_BYTE,
};

const unsigned VertexElementSize[]
{
    4,
    4,
    4 * 2,
    4 * 3,
    4 * 4,
    1
};

const unsigned VertexAttributeIdx[]
{
    0, // POSITION
    1, // NORMAL
    2, // TANGENT
    3, // VERTEX_COLOR
    4, // TEXCOORD
};

const std::string VertexAttributeName[]
{
    "aPosition",
    "aNormal",
    "aTangent",
    "aVertexColor",
    "aTexCoord"
};

const std::string VertexElementSemanticName[]
{
    "POSITION",
    "NORMAL",
    "TANGENT",
    "VERTEX_COLOR",
    "TEXCOORD",
    ""
};

const std::string PresetUniformName[]
{
    "uWorldMatrix",
    ""
};

std::string AttributesToString(unsigned attributes)
{
    return std::bitset<8>(attributes).to_string();
}

} // namespace Pt