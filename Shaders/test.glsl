#ifdef COMPILE_VS

layout (location = 0) in vec3 aPos;
out vec3 vPos;

#else

layout (location = 0) out vec4 FragColor;
in vec3 vPos;

#endif
    
void vert()
{
    gl_Position = vec4(aPos, 1.0);
    vPos = aPos;
}

void frag()
{
    FragColor = vec4(vPos, 1.0);
}