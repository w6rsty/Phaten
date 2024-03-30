const vec2 cResolution = vec2(1280, 720);
const float cAspect = cResolution.x / cResolution.y;
const float cFocusLength = 1.0;
const vec3 cCameraCenter = vec3(0, 0, 1);

const float MAX_DIST = 1000.0;

struct Ray
{
    vec3 origin;
    vec3 direction;
};

vec3 at(in Ray ray, float t)
{
    return ray.origin + t * ray.direction;
}

float iPlane(in Ray ray, in vec2 distBound, inout vec3 normal, in vec3 planeNormal, in float planeDist)
{
    float a = dot(ray.direction, planeNormal);
    float d = -(dot(ray.origin, planeNormal)+planeDist)/a;
    if (a > 0. || d < distBound.x || d > distBound.y) {
        return MAX_DIST;
    } else {
        normal = planeNormal;
    	return d;
    }
}


float iSphere(in Ray ray, in vec2 distBound, inout vec3 normal, in float radius, in vec3 center)
{
    vec3 oc = ray.origin - center;
    float b = dot(oc, ray.direction);
    float c = dot(oc, oc) - radius * radius;
    float h = b * b - c;
    if (h < 0.0)
    {
        return MAX_DIST;
    }
    else
    {
        h = sqrt(h);
        float d1 = -b - h;
        float d2 = -b + h;
        if (d1 >= distBound.x && d1 <= distBound.y)
        {
            normal = normalize(at(ray, d1));
            return d1;
        }
        else if (d2 >= distBound.x && d2 <= distBound.y)
        {
            normal = normalize(at(ray, d2));
            return d2;
        }
        else
        {
            return MAX_DIST;
        }
    }
}

float iBox(in Ray ray, in vec2 distBound, inout vec3 normal, in vec3 boxSize, in vec3 boxCenter)
{
    vec3 oc = ray.origin - boxCenter;
    vec3 m = sign(ray.direction)/max(abs(ray.direction), 1e-8);
    vec3 n = m*oc;
    vec3 k = abs(m)*boxSize;
	
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

	float tN = max(max(t1.x, t1.y), t1.z);
	float tF = min(min(t2.x, t2.y), t2.z);
	
    if (tN > tF || tF <= 0.) {
        return MAX_DIST;
    } else {
        if (tN >= distBound.x && tN <= distBound.y) {
        	normal = -sign(ray.direction)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
            return tN;
        } else if (tF >= distBound.x && tF <= distBound.y) { 
        	normal = -sign(ray.direction)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
            return tF;
        } else {
            return MAX_DIST;
        }
    }
}

float opU(float d, float result) {
    return min(d, result);
}

vec3 tracer(inout Ray ray)
{
    vec3 normal;
    vec2 dist = vec2(0.00, MAX_DIST);
    float d = MAX_DIST;

    d = opU(d, iPlane(ray, dist, normal, vec3(0, 1, 0), 1.0));
    d = opU(d, iBox(ray, dist, normal, vec3(0.15), vec3(-1.0, 0.1, -0.5)));
    d = opU(d, iSphere(ray, dist, normal, 0.20, vec3(0, 0.1, 0.2)));
    d = opU(d, iSphere(ray, dist, normal, 0.15, vec3(0, 0.1, 0.0)));

    if (d < MAX_DIST)
    {
        vec3 target = at(ray, d);

        vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0) - target);
        float diff = max(dot(normal, lightDir), 0.0);

        return diff * vec3(1.0, 1.0, 1.0);
    }

    vec3 unitDirection = normalize(ray.direction);
    float k = 0.5 * (unitDirection.y + 1.0);
    return (1.0f - k) * vec3(1.0f, 1.0f, 1.0f) + k * vec3(0.5f, 0.7f, 1.0f);
}
