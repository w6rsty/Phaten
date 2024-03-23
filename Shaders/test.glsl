// #include "Uniform.glsl"

layout (std140) uniform Color
{
    vec4 col;
};

#if defined(COMPILE_VS)

// #include "Transform.glsl"

layout (location = 0) in vec3 aPosition;
layout (location = 3) in vec3 aVertexColor;
out vec3 vPos;
out vec3 vColor;

#else

layout (location = 0) out vec4 FragColor;
in vec3 vPos;
in vec3 vColor;

#endif
    
void vert()
{
    gl_Position = vec4(aPosition, 1.0);
    vPos = aPosition;
    vColor = aVertexColor;
}

void frag()
{
    vec4 mid = col * vec4(vColor, 1.0);
    mid = mid * 0.5 + 0.5;
    FragColor = mid;
}