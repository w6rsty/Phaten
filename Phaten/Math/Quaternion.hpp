// TODO: Code review
#pragma once

#include "Math/Math.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"

namespace Pt {

class Quaternion
{
public:
    Quaternion()
    {
    }

    Quaternion(const Quaternion& quat) :
        w(quat.w),
        x(quat.x),
        y(quat.y),
        z(quat.z)
    {
    }

    Quaternion(float w_, float x_, float y_, float z_) :
        w(w_),
        x(x_),
        y(y_),
        z(z_)
    {
    }

    Quaternion(const float* data) :
        w(data[0]),
        x(data[1]),
        y(data[2]),
        z(data[3])
    {
    }

    /// Angle in degrees
    Quaternion(float angle, const Vector3& axis)
    {   
        FromAngleAxis(angle, axis);
    }

    /// Euler angles in degrees
    Quaternion(float x_, float y_, float z_)
    {
        FromEulerAngles(Vector3(x_, y_, z_));
    }

    /// Contruct from rotation matrix
    Quaternion(const Matrix3& mat)
    {
        FromMatrix(mat);
    }

    /// Construct from string
    Quaternion(std::string_view str)
    {
        FromString(str);
    }

    Quaternion& operator = (const Quaternion& rhs)
    {
        w = rhs.w;
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Quaternion& operator += (const Quaternion& rhs)
    {
        w += rhs.w;
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Quaternion& operator *= (float rhs)
    {
        w *= rhs;
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    bool operator == (const Quaternion& rhs) const { return w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator != (const Quaternion& rhs) const { return !(*this == rhs); }
    Quaternion operator * (float rhs) const { return Quaternion(w * rhs, x * rhs, y * rhs, z * rhs); }
    Quaternion operator + (const Quaternion& rhs) const { return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z); }
    Quaternion operator - () const { return Quaternion(-w, -x, -y, -z); }
    Quaternion operator - (const Quaternion& rhs) const { return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z); }

    Quaternion operator * (const Quaternion& rhs) const
    {
        return Quaternion(
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
            w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w
        );
    }

    Vector3 operator * (const Vector3& rhs) const
    {
        Quaternion q = *this * Quaternion(0, rhs.x, rhs.y, rhs.z) * Conjugate();
        return Vector3(q.x, q.y, q.z);
    }

    void FromAngleAxis(float angle, const Vector3& axis);
    /// From euler angles in degrees
    void FromEulerAngles(const Vector3& euler);
    /// From rotation matrix
    void FromMatrix(const Matrix3& mat);
    /// From string
    bool FromString(std::string_view str);

    void Normalize()
    {
        float lenSquared = LengthSquared();
        if (!::EpsilonEqual(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            w *= invLen;
            x *= invLen;
            y *= invLen;
            z *= invLen;
        }
    }

    Quaternion Normalized() const
    {
        float lenSquared = LengthSquared();
        if (!::EpsilonEqual(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            return *this * invLen;
        }
        else
        {
            return *this;
        }
    }

    Quaternion Inverse() const
    {
        float lenSquared = LengthSquared();
        if (!::EpsilonEqual(lenSquared, 1.0f))
        {
            return Conjugate();
        }
        else if (lenSquared > ::M_EPSILON)
        {
            return Conjugate() * (1.0f / lenSquared);
        }
        else
        {
            return Quaternion::IDENTITY;
        }
    }

    float LengthSquared() const { return w * w + x * x + y * y + z * z; }
    float Dot(const Quaternion& rhs) const { return w * rhs.w + x * rhs.x + y * rhs.y + z * rhs.z; }
    bool Equals(const Quaternion& rhs, float epsilon = ::M_EPSILON) const { return ::EpsilonEqual(w, rhs.w, epsilon) && ::EpsilonEqual(x, rhs.x, epsilon) && ::EpsilonEqual(y, rhs.y, epsilon) && ::EpsilonEqual(z, rhs.z, epsilon); }
    bool IsNaN() const { return ::IsNaN(w) || ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z); }
    Quaternion Conjugate() const { return Quaternion(w, -x, -y, -z); }

    Vector3 EulerAngles() const;

    Matrix3 ToMatrix() const
    {
        return Matrix3(
            1.0f - 2.0f * y * y - 2.0f * z * z,
            2.0f * x * y - 2.0f * w * z,
            2.0f * x * z + 2.0f * w * y,
            2.0f * x * y + 2.0f * w * z,
            1.0f - 2.0f * x * x - 2.0f * z * z,
            2.0f * y * z - 2.0f * w * x,
            2.0f * x * z - 2.0f * w * y,
            2.0f * y * z + 2.0f * w * x,
            1.0f - 2.0f * x * x - 2.0f * y * y
        );
    }

    Quaternion Slerp(Quaternion rhs, float t) const;

    const float* Data() const { return data; }

    std::string ToString() const;

    union
    {
        struct
        {
            float w, x, y, z;
        };
        float data[4];
    };

    static const Quaternion IDENTITY;
};

} // namespace Pt