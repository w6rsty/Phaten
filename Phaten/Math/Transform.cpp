#include "Transform.hpp"

#include "Math.hpp"

namespace Pt {

Matrix3 ScaleMatrix3(const Vector3& scale)
{
    Matrix3 ret = Matrix3::IDENTITY;
    ret.data[0][0] = scale.x;
    ret.data[1][1] = scale.y;
    ret.data[2][2] = scale.z;
    return ret;
}

Matrix3 ScaleMatrix3(float scale)
{
    return ScaleMatrix3(Vector3{scale, scale, scale});
}

Matrix3 RotationMatrix3(float degree, const Vector3& rotation)
{
    Matrix3 ret = Matrix3::IDENTITY;
    float rad = ::Radians(degree);
    float c = cosf(rad);
    float s = sinf(rad);
    float t = 1.0f - c;

    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;

    ret.data[0][0] = t * x * x + c;
    ret.data[0][1] = t * x * y + s * z;
    ret.data[0][2] = t * x * z - s * y;

    ret.data[1][0] = t * x * y - s * z;
    ret.data[1][1] = t * y * y + c;
    ret.data[1][2] = t * y * z + s * x;

    ret.data[2][0] = t * x * z + s * y;
    ret.data[2][1] = t * y * z - s * x;
    ret.data[2][2] = t * z * z + c;

    return ret;
}

Matrix3 RotationMatrix3(const Vector3& rotation)
{
    Matrix3 ret = Matrix3::IDENTITY;
    float radX = ::Radians(rotation.x);
    float radY = ::Radians(rotation.y);
    float radZ = ::Radians(rotation.z);

    float cx = cosf(radX);
    float sx = sinf(radX);
    float cy = cosf(radY);
    float sy = sinf(radY);
    float cz = cosf(radZ);
    float sz = sinf(radZ);

    ret.data[0][0] = cy * cz;
    ret.data[0][1] = cy * sz;
    ret.data[0][2] = -sy;

    ret.data[1][0] = sx * sy * cz - cx * sz;
    ret.data[1][1] = sx * sy * sz + cx * cz;
    ret.data[1][2] = sx * cy;

    ret.data[2][0] = cx * sy * cz + sx * sz;
    ret.data[2][1] = cx * sy * sz - sx * cz;
    ret.data[2][2] = cx * cy;

    return ret;
}

Matrix3 RotationMatrix3(float degreeX, float degreeY, float degreeZ)
{
    return RotationMatrix3(Vector3{degreeX, degreeY, degreeZ});
}


Matrix4 ScaleMatrix4(const Vector3& scale)
{
    Matrix4 ret = Matrix4::IDENTITY;
    ret.data[0][0] = scale.x;
    ret.data[1][1] = scale.y;
    ret.data[2][2] = scale.z;
    ret.data[3][3] = 1.0f;
    return ret;
}

Matrix4 ScaleMatrix4(float x, float y, float z)
{
    return ScaleMatrix4(Vector3{x, y, z});
}

Matrix4 RotationMatrix4(float degree, const Vector3 &rotation)
{
    Matrix4 ret = RotationMatrix3(degree, rotation);
    return ret;
}

Matrix4 RotationMatrix4(const Vector3& rotation)
{
    Matrix4 ret = RotationMatrix3(rotation);
    return ret;
}

Matrix4 RotationMatrix4(float degreeX, float degreeY, float degreeZ)
{
    return RotationMatrix4(Vector3{degreeX, degreeY, degreeZ});
}

Matrix4 TranslateMatrix4(const Vector3& translation)
{
    Matrix4 ret = Matrix4::IDENTITY;
    ret.data[3][0] = translation.x;
    ret.data[3][1] = translation.y;
    ret.data[3][2] = translation.z;
    return ret;
}

Matrix4 TranslateMatrix4(float x, float y, float z)
{
    return TranslateMatrix4(Vector3{x, y, z});
}

bool DecomposeSRT(const Matrix4 &matrix, Vector3 &scale, Vector3 &rotation, Vector3 &translation)
{
    Matrix4 localMatrix = matrix;

   if (::EpsilonEqual(localMatrix[3][3], 0.0f))
        return false;

    if (::EpsilonNotEqual(localMatrix[0][3], 0.0f) ||
        ::EpsilonNotEqual(localMatrix[1][3], 0.0f) ||
        ::EpsilonNotEqual(localMatrix[2][3], 0.0f))
    {
        localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
        localMatrix[3][3] = 1.0f;
    }

    translation = Vector3{localMatrix[3][0], localMatrix[3][1], localMatrix[3][2]};
    localMatrix[3][0] = localMatrix[3][1] = localMatrix[3][2] = 0.0f;

    scale.x = Vector3{localMatrix[0][0], localMatrix[0][1], localMatrix[0][2]}.Length();
    scale.y = Vector3{localMatrix[1][0], localMatrix[1][1], localMatrix[1][2]}.Length();
    scale.z = Vector3{localMatrix[2][0], localMatrix[2][1], localMatrix[2][2]}.Length();

    if (::EpsilonEqual(scale.x, 0.0f) || ::EpsilonEqual(scale.y, 0.0f) || ::EpsilonEqual(scale.z, 0.0f))
    {
        scale = Vector3::ONE;
        return false;
    }

    localMatrix[0][0] /= scale.x;
    localMatrix[0][1] /= scale.x;
    localMatrix[0][2] /= scale.x;

    localMatrix[1][0] /= scale.y;
    localMatrix[1][1] /= scale.y;
    localMatrix[1][2] /= scale.y;

    localMatrix[2][0] /= scale.z;
    localMatrix[2][1] /= scale.z;
    localMatrix[2][2] /= scale.z;

    rotation.y = asinf(-localMatrix[0][2]);
    if (::EpsilonEqual(cosf(rotation.y), 0.0f))
    {
        rotation.x = atan2f(localMatrix[1][0], localMatrix[1][1]);
        rotation.z = 0.0f;
    }
    else
    {
        rotation.x = atan2f(localMatrix[1][2], localMatrix[2][2]);
        rotation.z = atan2f(localMatrix[0][1], localMatrix[0][0]);
    }

    return true;
}

} // namespace Pt