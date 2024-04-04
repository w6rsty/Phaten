#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 4) in vec2 aTexCoord;
out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCoord;

#else

#include "Phaten/Shaders/Common.glsl"

layout (location = 0) out vec4 FragColor;
in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCoord;

#endif

void vert()
{
    vec4 pos = uModel * vec4(aPosition, 1.0);
    vPos = pos.xyz;
    vNormal = (uModel * vec4(aNormal, 0.0)).xyz;
    vTexCoord = aTexCoord;
    gl_Position = uProjection * uView * pos;
}

void frag()
{
    vec3 texColor = texture(uTexture0, vTexCoord).rgb;
    FragColor = vec4(texColor, 1.0);
}