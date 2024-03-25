#include "Phaten/Shaders/Common.glsl"
#include "Phaten/Shaders/Uniform.glsl"

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
    vec3 pixelCenter = vec3(vPos.x, vPos.y / iAspect, iCameraCenter.z - iFocusLength);

    Ray ray;
    ray.origin = iCameraCenter;
    ray.direction = normalize(pixelCenter - iCameraCenter);

    vec3 finalColor = vec3(0.0);
    finalColor = ray_color(ray);

    FragColor = vec4(finalColor, 1.0);
}