#include "Space.hpp"

#include "Math/Math.hpp"
#include "Math/Matrix.hpp"

namespace Pt {

void Perspective(Matrix4& mat, float fov, float aspect, float near, float far)
{
    mat = Matrix4(0.0f); // reset
    float tanHalfFov = tan(Radians(fov) * 0.5f);

    mat[0][0] = 1.0f / (aspect * tanHalfFov);
    mat[1][1] = 1.0f / tanHalfFov;
    mat[2][2] = (far + near) / (near - far);
    mat[2][3] = -1.0f;
    mat[3][2] = (2.0f * far * near) / (near - far);
}

Matrix4 Perspective(float fov, float aspect, float near, float far)
{
    Matrix4 ret;

    Perspective(ret, fov, aspect, near, far);

    return ret;
}

void Orthographic(Matrix4& mat, float size, float aspect, float near, float far)
{
    float left = -size * aspect * 0.5f;
    float right = size * aspect * 0.5f;
    float bottom = -size * 0.5f;
    float top = size * 0.5f;

    mat[0][0] = 2.0f / (right - left);
    mat[1][1] = 2.0f / (top - bottom);
    mat[2][2] = 2.0f / (far - near);
    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);
}

Matrix4 Orthographic(float size, float aspect, float near, float far)
{
    Matrix4 ret;

    Orthographic(ret, size, aspect, near, far);

    return ret;
}

void LookAt(Matrix4& mat, const Vector3& from, const Vector3& to, const Vector3& up)
{
    Vector3 f = Normalize(to - from);
    Vector3 s = Normalize(Cross(f, up));
    Vector3 u = Normalize(Cross(s, f));

    mat = Matrix4(1.0f); // reset
    mat[0][0] = s.x;
    mat[1][0] = s.y;
    mat[2][0] = s.z;
    mat[0][1] = u.x;
    mat[1][1] = u.y;
    mat[2][1] = u.z;
    mat[0][2] =-f.x;
    mat[1][2] =-f.y;
    mat[2][2] =-f.z;
    mat[3][0] =-Dot(s, from);
    mat[3][1] =-Dot(u, from);
    mat[3][2] = Dot(f, from);
}

Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up)
{
    Matrix4 ret (1.0f);

    LookAt(ret, from, to, up);

    return ret;
}

} // namespace Pt