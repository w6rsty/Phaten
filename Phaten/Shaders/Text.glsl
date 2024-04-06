#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
out vec2 vTexCoord;

#else

layout (location = 0) out vec4 FragColor;
in vec2 vTexCoord;

#endif

void vert()
{
    float x = float(int(aPosition.z) % 10);
    float y = float(int(aPosition.z) / 10);
    float corner = int(gl_VertexID) % 4;

    float xOffset = (corner == 1 || corner == 2) ? x + 1 : x;
    float yOffset = (corner == 2 || corner == 3) ? y + 1 : y;

    vTexCoord = vec2(xOffset * 16.0 / 160.0, yOffset * 16.0 / 160.0);
    gl_Position = uProjection * vec4(aPosition.xy, 0.0, 1.0);
}

void frag()
{
    float r = texture(uTexture0, vTexCoord).r;
    vec3 color = vec3(r);
    FragColor = vec4(color, 1.0);
}