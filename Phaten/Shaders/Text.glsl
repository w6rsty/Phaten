#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
out vec2 vTexCoord;

const float SHEET_GTRID_X = 10.0;
const float SHEET_GTRID_Y = 10.0;

vec2 FontTexCoord(float index, float vertexID)
{
    // Find row and column in the font sheet
    float x = float(int(index) % int(SHEET_GTRID_X));
    float y = 10 - float(int(index) / int(SHEET_GTRID_Y)) - 1;
    // Find corner by index
    float corner = int(vertexID) % 4;
    // Calculate texture quad coordinates
    float xOffset = (corner == 1 || corner == 2) ? x + 1 : x;
    float yOffset = (corner == 2 || corner == 3) ? y + 1 : y;
    return vec2(xOffset / SHEET_GTRID_X, yOffset / SHEET_GTRID_Y);
}

#else

layout (location = 0) out vec4 FragColor;
in vec2 vTexCoord;

#endif

void vert()
{
    vTexCoord = FontTexCoord(aPosition.z, gl_VertexID);
    gl_Position = vec4(aPosition.xy, 0.0, 1.0);
}

void frag()
{
    vec4 color = texture(uTexture0, vTexCoord);
    if (color.a < 0.1)
        discard;
    FragColor = vec4(color.rgb, 1.0);
}