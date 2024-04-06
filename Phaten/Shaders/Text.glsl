#include "Phaten/Shaders/Uniform.glsl"
#include "Phaten/Shaders/TextCommon.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
out float vFontIndex;
out vec2 vTexCoord;

#else

layout (location = 0) out vec4 FragColor;
in float vFontIndex;
in vec2 vTexCoord;

#endif

void vert()
{
    vFontIndex = aPosition.z;
    vTexCoord = GetFontTexPos(vFontIndex, gl_VertexID);
    gl_Position = uProjection * vec4(aPosition.xy, 0.0, 1.0);
}

void frag()
{
    float r = texture(uTexture0, vTexCoord).r;
    vec3 color = vec3(r);
    FragColor = vec4(color, 1.0);
}