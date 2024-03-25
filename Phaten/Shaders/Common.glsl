const vec2 iResolution = vec2(800, 600);
const float iAspect = iResolution.x / iResolution.y;

const float iFocusLength = 1.0;

const vec3 iCameraCenter = vec3(0, 0, 1);

struct Ray
{
    vec3 origin;
    vec3 direction;
};

vec3 at(in Ray ray, float t)
{
    return ray.origin + t * ray.direction;
}

float hit_sphere(inout vec3 center, float radius, Ray r)
{
    vec3 oc = r.origin - center;
    float a = dot(r.direction, r.direction);
    float b = 2.0f * dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4.0 * a * c;

    if (discriminant < 0)
    {
        return -1.0;
    }
    else
    {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 ray_color(inout Ray ray)
{
    vec3 sphereCenter = vec3(0, 0, -1);
    float t = hit_sphere(sphereCenter, 1.0, ray);
    if (t > 0.0)
    {
        vec3 N = normalize(at(ray, t) - vec3(0, 0, -1));
        return 0.5 * (N + vec3(1.0));
    }

    vec3 unitDirection = normalize(ray.direction);
    float k = 0.5 * (unitDirection.y + 1.0);
    return (1.0f - k) * vec3(1.0f, 1.0f, 1.0f) + k * vec3(0.5f, 0.7f, 1.0f);
}