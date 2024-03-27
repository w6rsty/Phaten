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
    "uTime",
    "uWorldMatrix",
    ""
};

const std::string PrimitiveName[]
{
    "POINTS",
    "LINES",
    "LINE_STRIP",
    "TRIANGLES",
    "TRIANGLE_STRIP",
    "TRIANGLE_FAN"
};

const GLenum PrimitiveGLType[]
{
    GL_POINTS,
    GL_LINES,
    GL_LINE_STRIP,
    GL_TRIANGLES,
    GL_TRIANGLE_STRIP,
    GL_TRIANGLE_FAN
};


std::string AttributesBitToString(unsigned attributes)
{
    return std::bitset<8>(attributes).to_string();
}

std::string ShowAttributes(unsigned attributes)
{
    std::string result = "";
    int max = static_cast<int>(VertexAttributeType::MAX_ATTRIBUTE);
    for (int i = 0; i < max; ++i)
    {
        if (attributes & (1 << i))
        {
            result += " " + VertexAttributeName[i] ;
        }
    }
    return result;
}

} // namespace Pt