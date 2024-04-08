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

const float pb = 0.5; // affect brightness
const float sclV = 0.0; // scanline brightness

#endif

void vert()
{
    vPos = aPosition;
    vTexCoord = aTexCoord;
    gl_Position = vec4(aPosition.x, aPosition.y, 0.0, 1.0);
}

void frag()
{
#if defined(ENABLE)
    vec3 texColor = texture(uTexture2, vTexCoord).rgb;

    vec3 lcdColor = vec3(pb);

    int px = int(mod(gl_FragCoord.x, 3.0));
    int py = int(mod(gl_FragCoord.y, 3.0));

	if (px == 1) lcdColor.r = 1.0;
    else if (px == 2) lcdColor.g = 1.0;
    else lcdColor.b = 1.0;

    if (py == 0) lcdColor.rgb = vec3(sclV);

    vec3 col = texColor * lcdColor;
#else
    vec3 col = texture(uTexture2, vTexCoord).rgb;
#endif
    FragColor = vec4(col, 1.0);
}