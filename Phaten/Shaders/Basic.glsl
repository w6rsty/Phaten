#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 4) in vec2 aTexCoord;
out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;

#else

#include "Phaten/Shaders/Common.glsl"

layout (location = 0) out vec4 FragColor;
in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

#endif

void vert()
{
    vec4 pos = uModel * vec4(aPosition, 1.0);
    vPosition = pos.xyz;
    vNormal = aNormal;
#if defined(TILED)
    vTexCoord = aTexCoord * vec2(uModel[0][0], uModel[2][2]);
#else
    vTexCoord = aTexCoord;
#endif
    gl_Position = uProjection * uView * pos;
}

void frag()
{
#if defined(TEXTURE)
    vec4 color = texture(uTexture1, vTexCoord);
    if (color.a < 0.1)
        discard;
#else
    /// No texture warning, you will always use texture, right? :D
    vec4 color = vec4(1.0, 0.0, 1.0, 1.0);
#endif
    FragColor = vec4(color.rgb, 1.0);
}