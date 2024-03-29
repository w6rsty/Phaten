#include "Space.hpp"

#include "Math/Math.hpp"
#include "Math/Matrix.hpp"

namespace Pt {

void Perspective(Matrix4& mat, float fov, float aspect, float near, float far)
{
    float focal = 1.0f / std::tanf(Radians(fov) * 0.5f);

    mat[0][0] = focal / aspect;
    mat[1][1] = focal;
    mat[2][2] = 2.0f * near / (far - near);
    mat[3][2] = (2.0f * near * far) / (far - near);
    mat[2][3] = -1.0f;
}

Matrix4 Perspective(float fov, float aspect, float near, float far)
{
    Matrix4 ret (1.0f);

    Perspective(ret, fov, aspect, near, far);

    return ret;
}

void Orthographic(Matrix4& mat, float left, float right, float bottom, float top, float near, float far)
{
    mat[0][0] = 2.0f / (right - left);
    mat[1][1] = 2.0f / (top - bottom);
    mat[2][2] = -2.0f / (far - near);
    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);
}

Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far)
{
    Matrix4 ret (1.0f);

    Orthographic(ret, left, right, bottom, top, near, far);

    return ret;
}

void LookAt(Matrix4& mat, const Vector3& from, const Vector3& to, const Vector3& up)
{
    Vector3 D = Normalize(from - to);
    Vector3 R = Normalize(Cross(up, D));
    Vector3 U = Normalize(Cross(D, R));

    mat[0][0] = R.x;
    mat[0][1] = R.y;
    mat[0][2] = R.z;

    mat[1][0] = U.x;
    mat[1][1] = U.y;
    mat[1][2] = U.z;

    mat[2][0] = D.x;
    mat[2][1] = D.y;
    mat[2][2] = D.z;

    mat[3][0] = -Dot(R, from);
    mat[3][1] = -Dot(U, from);
    mat[3][2] = -Dot(D, from);
}

Matrix4 LookAt(const Vector3& from, const Vector3& to, const Vector3& up)
{
    Matrix4 ret (1.0f);

    LookAt(ret, from, to, up);

    return ret;
}

} // namespace Pt