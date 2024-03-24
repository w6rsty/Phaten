#include "Shaders/Uniform.glsl"

layout (std140) uniform Color
{
    vec4 col;
};

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
out vec3 vPos;

#else

layout (location = 0) out vec4 FragColor;
in vec3 vPos;

#endif
    
void vert()
{
    gl_Position = vec4(aPosition, 1.0);
    vPos = aPosition;
}

void frag()
{
    FragColor = vec4(vPos, 1.0);
}