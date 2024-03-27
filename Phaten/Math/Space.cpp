#include "Space.hpp"

namespace Pt {

Matrix4 Perspective(float fov, float aspect, float near, float far)
{
    float tanHalfFov = tan(fov / 2.0f);
    float zRange = near - far;

    return Matrix4{
        1.0f / (tanHalfFov * aspect), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
        0.0f, 0.0f, (-near - far) / zRange, 2.0f * far * near / zRange,
        0.0f, 0.0f, 1.0f, 0.0f
    };
}

Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far)
{
    float xRange = right - left;
    float yRange = top - bottom;
    float zRange = far - near;

    return Matrix4{
        2.0f / xRange, 0.0f, 0.0f, -(right + left) / xRange,
        0.0f, 2.0f / yRange, 0.0f, -(top + bottom) / yRange,
        0.0f, 0.0f, -2.0f / zRange, -(far + near) / zRange,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    Vector3 forward = Normalize(target - eye);
    Vector3 right = Normalize(Cross(forward, up));
    Vector3 newUp = Cross(right, forward);

    return Matrix4{
        right.x, newUp.x, -forward.x, 0.0f,
        right.y, newUp.y, -forward.y, 0.0f,
        right.z, newUp.z, -forward.z, 0.0f,
        -Dot(right, eye), -Dot(newUp, eye), Dot(forward, eye), 1.0f
    };
}

} // namespace Pt