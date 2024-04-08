#include "Quaternion.hpp"

#include "IO/StringUtils.hpp"
#include "Math/Math.hpp"

namespace Pt {

const Quaternion Quaternion::IDENTITY { 1.0f, 0.0f, 0.0f, 0.0f };

void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
{
    float halfAngle = angle * 0.5f;
    float s = sinf(halfAngle);
    w = cosf(halfAngle);
    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
}

void Quaternion::FromEulerAngles(const Vector3& euler)
{
    Vector3 e = euler * 0.5f;

    float cx = cosf(e.x);
    float sx = sinf(e.x);
    float cy = cosf(e.y);
    float sy = sinf(e.y);
    float cz = cosf(e.z);
    float sz = sinf(e.z);

    w = cx * cy * cz + sx * sy * sz;
    x = sx * cy * cz - cx * sy * sz;
    y = cx * sy * cz + sx * cy * sz;
    z = cx * cy * sz - sx * sy * cz;
}

void Quaternion::FromMatrix(const Matrix3& mat)
{
    float t = mat.Trace();

    if (t > 0.0f)
    {
        float invS = 0.5f / sqrtf(t + 1.0f);

        x= mat[2][1] - mat[1][2] * invS;
        y = mat[0][2] - mat[2][0] * invS;
        z = mat[1][0] - mat[0][1] * invS;
        w = 0.25f / invS;
    }
    else
    {
        if (mat[0][0] > mat[1][1] && mat[0][0] > mat[2][2])
        {
            float invS = 0.5f / sqrtf(1.0f + mat[0][0] - mat[1][1] - mat[2][2]);

            x = 0.25f / invS;
            y = (mat[0][1] + mat[1][0]) * invS;
            z = (mat[0][2] + mat[2][0]) * invS;
            w = (mat[2][1] - mat[1][2]) * invS;
        }
        else if (mat[1][1] > mat[2][2])
        {
            float invS = 0.5f / sqrtf(1.0f + mat[1][1] - mat[0][0] - mat[2][2]);

            x = (mat[0][1] + mat[1][0]) * invS;
            y = 0.25f / invS;
            z = (mat[1][2] + mat[2][1]) * invS;
            w = (mat[0][2] - mat[2][0]) * invS;
        }
        else
        {
            float invS = 0.5f / sqrtf(1.0f + mat[2][2] - mat[0][0] - mat[1][1]);

            x = (mat[0][2] + mat[2][0]) * invS;
            y = (mat[1][2] + mat[2][1]) * invS;
            z = 0.25f / invS;
            w = (mat[1][0] - mat[0][1]) * invS;
        }
    }
}

bool Quaternion::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');

    if (count != 4)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    w = strtof(ptr, &ptr);
    x = strtof(ptr, &ptr);
    y = strtof(ptr, &ptr);
    z = strtof(ptr, &ptr);
    return true;
}

Vector3 Quaternion::EulerAngles() const
{
    // Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
    // Order of rotations: Z first, then X, then Y
    float check = 2.0f * (-y * z + w * x);
    
    if (check < -0.995f)
    {
        return Vector3(
            -90.0f,
            0.0f,
            -atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * ::M_RAD_TO_DEG
        );
    }
    else if (check > 0.995f)
    {
        return Vector3(
            90.0f,
            0.0f,
            atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)) * ::M_RAD_TO_DEG
        );
    }
    else
    {
        return Vector3(
            asinf(check) * ::M_RAD_TO_DEG,
            atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)) * M_RAD_TO_DEG,
            atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)) * M_RAD_TO_DEG
        );
    }
}

Quaternion Quaternion::Slerp(Quaternion rhs, float t) const
{
    float cosAngle = Dot(rhs);
    // Enable shortest path rotation
    if (cosAngle < 0.0f)
    {
        cosAngle = -cosAngle;
        rhs = -rhs;
    }
    
    float angle = acosf(cosAngle);
    float sinAngle = sinf(angle);
    float t1, t2;
    
    if (sinAngle > 0.001f)
    {
        float invSinAngle = 1.0f / sinAngle;
        t1 = sinf((1.0f - t) * angle) * invSinAngle;
        t2 = sinf(t * angle) * invSinAngle;
    }
    else
    {
        t1 = 1.0f - t;
        t2 = t;
    }
    
    return *this * t1 + rhs * t2;
}

std::string Quaternion::ToString() const
{
    return FormatString("%f, %f, %f, %f", w, x, y, z);
}

} // namespace Pt