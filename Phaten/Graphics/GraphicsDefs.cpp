#include "GraphicsDefs.hpp"

#include <bitset>

#include <glad/glad.h>

namespace Pt {

const unsigned BufferBitGLType[]
{
    GL_COLOR_BUFFER_BIT,
    GL_DEPTH_BUFFER_BIT,
    GL_STENCIL_BUFFER_BIT
};

unsigned BufferBitsToGLBits(unsigned bits)
{
    unsigned result = 0;
    for (int i = 0; i < static_cast<int>(BufferBitType::MAX_BUFFER_BIT); ++i)
    {
        if (bits & (1 << i))
        {
            result |= BufferBitGLType[i];
        }
    }
    return result;
}


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
    4, // TEX_COORD
};

const std::string VertexAttributeName[]
{
    "aPosition",
    "aNormal",
    "aTangent",
    "aVertexColor",
    "aTexCoord",
};

const std::string VertexElementSemanticName[]
{
    "POSITION",
    "NORMAL",
    "TANGENT",
    "VERTEX_COLOR",
    "TEX_COORD",
    ""
};

const std::string PresetUniformName[]
{
    "uTime",
    "uModel",
    "uCameraPosition",
    "uSkybox",
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

const GLenum ImageFormatGLInternalFormat[]
{
    0,
    GL_R8,
    GL_RG8,
    GL_RGB8,
    GL_RGBA8,
    GL_ALPHA,
    GL_R16,
    GL_RG16,
    GL_RGBA16,
    GL_R16F,
    GL_RG16F,
    GL_RGBA16F,
    GL_R32F,
    GL_RG32F,
    GL_RGB32F,
    GL_RGBA32F,
    GL_R32UI,
    GL_RG32UI,
    GL_RGBA32UI,
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT32,
    GL_DEPTH24_STENCIL8,
};

const GLenum ImageFormatGLFormat[]
{
    0,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_ALPHA,
    GL_RED,
    GL_RG,
    GL_RGBA,
    GL_RED,
    GL_RG,
    GL_RGBA,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_RED_INTEGER,
    GL_RG_INTEGER,
    GL_RGBA_INTEGER,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_COMPONENT,
    GL_DEPTH_STENCIL,
};

const GLenum ImageFormatGLDataType[]
{
    0,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_SHORT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT_24_8,
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

const GLenum TextureTypeGLTarget[]
{
    GL_TEXTURE_2D,
    GL_TEXTURE_3D,
    GL_TEXTURE_CUBE_MAP
};

const GLenum TextureWrapModeGLType[]
{
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER
};

const GLenum TextureFilterModeGLType[]
{
    GL_NEAREST,
    GL_LINEAR
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