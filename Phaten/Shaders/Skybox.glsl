#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 4) in vec2 aTexCoord;
out vec3 vPosition;
out vec3 vNormal;

#else

#include "Phaten/Shaders/Common.glsl"

layout (location = 0) out vec4 FragColor;
in vec3 vPosition;
in vec3 vNormal;

#endif

void vert()
{
    vPosition = aPosition;
    vNormal = aNormal;
    gl_Position = (uProjection * mat4(mat3(uView)) * vec4(aPosition, 1.0)).xyww;
}

void frag()
{
    float exposure = uDragFloat;
    vec3 env = texture(uSkybox, vPosition).rgb;
    env = env * pow(2.0, exposure);
    FragColor = vec4(env, 1.0);
}