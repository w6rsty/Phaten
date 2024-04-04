#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 4) in vec2 aTexCoord;
out vec3 vPos;
out vec2 vTexCoord;

#else

#include "Phaten/Shaders/Common.glsl"

layout (location = 0) out vec4 FragColor;
in vec3 vPos;
in vec2 vTexCoord;

#endif

void vert()
{
    gl_Position = uProjectionView * uModel * vec4(aPosition, 1.0);
    vPos = aPosition;
    vTexCoord = aTexCoord;
}

void frag()
{
    vec3 texColor = texture(uTexture0, vTexCoord).rgb;
    FragColor = vec4(texColor, 1.0);
}