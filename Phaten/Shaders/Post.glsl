#include "Phaten/Shaders/Uniform.glsl"

#if defined(COMPILE_VS)

layout (location = 0) in vec3 aPosition;
layout (location = 4) in vec2  aTexCoord;
out vec3 vPos;
out vec2 vTexCoord;

#else

#include "Phaten/Shaders/Common.glsl"

layout (location = 0) out vec4 FragColor;
in vec3 vPos;
in vec2 vTexCoord;

const float offset = 1.0 / 200.0;

// Gaussian blur
const float kernel[9] = float[](
    1.0 / 16.0, 2.0 / 16.0, 1.0/ 16.0,
    2.0 / 16.0, 4.0 / 16.0, 2.0/ 16.0,
    1.0 / 16.0, 2.0 / 16.0, 1.0/ 16.0
);

#endif

void vert()
{
    vPos = aPosition;
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}

void frag()
{

#if defined(KERNEL)
    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2(0.0f, offset), // top-center
        vec2(offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2(0.0f, 0.0f), // center-center
        vec2(offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2(0.0f, -offset), // bottom-center
        vec2(offset, -offset) // bottom-right
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = texture(uTexture1, vTexCoord + offsets[i]).rgb;
    }

    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        col += sampleTex[i] * kernel[i];
    }
#else
    vec3 col = texture(uTexture1, vTexCoord).rgb;
#endif
    FragColor = vec4(col, 1.0);
}