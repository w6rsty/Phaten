#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 4) in vec2  aTexCoord;
out vec3 vPos;

#else

layout (location = 0) out vec4 FragColor;
in vec3 vPos;

#endif

void vert()
{
    gl_Position = uProjectionView * vec4(aPosition, 1.0);
    vPos = aPosition;
}

void frag()
{
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}