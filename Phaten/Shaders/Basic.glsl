#include "Phaten/Shaders/Common.glsl"
#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)
layout (location = 0) in vec3 aPosition;
layout (location = 4) in vec2 aTexCoord;
out vec3 vPos;
out vec2 vTexCoord;
#else
layout (location = 0) out vec4 FragColor;
in vec3 vPos;
in vec2 vTexCoord;
#endif

void vert()
{
    gl_Position = vec4(aPosition, 1.0);
    vPos = aPosition;
    vTexCoord = aTexCoord;
}

void frag()
{
    vec2 uv = vTexCoord * 2.0 - 1.0;
    uv.y /= iAspect;

    vec3 finalColor = vec3(1.0, 0.0, 1.0);
    
    FragColor = vec4(finalColor, 1.0);
}