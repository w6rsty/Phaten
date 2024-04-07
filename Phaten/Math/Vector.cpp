#include "Vector.hpp"

#include <string>

#include "IO/StringUtils.hpp"
#include "IntVector.hpp"

namespace Pt {

const Vector2 Vector2::ZERO     {0, 0};
const Vector2 Vector2::ONE      {1, 1};
const Vector2 Vector2::X        {1, 0};
const Vector2 Vector2::Y        {0, 1};

Vector2::Vector2(const IntV2& vector) :
    x(static_cast<float>(vector.x)),
    y(static_cast<float>(vector.y))
{
}

bool Vector2::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');

    if (count != 2)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    x = strtof(ptr, &ptr);
    y = strtof(ptr, &ptr);
    return true;
}

Vector2 operator * (float lhs, const Vector2& rhs)
{
    return rhs * lhs;
}

Vector3& Vector3::operator = (const Vector4& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

std::string Vector2::ToString() const
{
    return FormatString("%f %f", x, y);
}

const Vector3 Vector3::ZERO     {0, 0, 0};
const Vector3 Vector3::ONE      {1, 1, 1};
const Vector3 Vector3::X        {1, 0, 0};
const Vector3 Vector3::Y        {0, 1, 0};
const Vector3 Vector3::Z        {0, 0, 1};

const Vector3 Vector3::UP         { 0,  1,  0};
const Vector3 Vector3::DOWN       { 0, -1,  0};
const Vector3 Vector3::RIGHT      { 1,  0,  0};
const Vector3 Vector3::LEFT       {-1,  0,  0};
const Vector3 Vector3::FORWARD    { 0,  0,  1};
const Vector3 Vector3::BACKWARD   { 0,  0, -1};

Vector3::Vector3(const Vector4& vector) :
    x(vector.x),
    y(vector.y),
    z(vector.z)
{
}

Vector3::Vector3(const IntV3& vector) :
    x(static_cast<float>(vector.x)),
    y(static_cast<float>(vector.y)),
    z(static_cast<float>(vector.z))
{
}

bool Vector3::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');

    if (count != 3)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    x = strtof(ptr, &ptr);
    y = strtof(ptr, &ptr);
    z = strtof(ptr, &ptr);
    return true;
}

std::string Vector3::ToString() const
{
    return FormatString("%f %f %f", x, y, z);
}

Vector3 operator * (float lhs, const Vector3& rhs)
{
    return rhs * lhs;
}

const Vector4 Vector4::ZERO     {0, 0, 0, 0};
const Vector4 Vector4::ONE      {1, 1, 1, 1};
const Vector4 Vector4::X        {1, 0, 0, 0};
const Vector4 Vector4::Y        {0, 1, 0, 0};
const Vector4 Vector4::Z        {0, 0, 1, 0};
const Vector4 Vector4::W        {0, 0, 0, 1};

bool Vector4::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');

    if (count != 4)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    x = strtof(ptr, &ptr);
    y = strtof(ptr, &ptr);
    z = strtof(ptr, &ptr);
    w = strtof(ptr, &ptr);
    return true;
}

std::string Vector4::ToString() const
{
    return FormatString("%f %f %f %f", x, y, z, w);
}

Vector4 operator * (float lhs, const Vector4& rhs)
{
    return rhs * lhs;
}

// ============================================================================
// ============================================================================

float Dot(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}
Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
{
    return Vector3{
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

Vector3 Normalize(const Vector3& vec)
{
    float length = vec.Length();
    return Vector3{vec.x / length, vec.y / length, vec.z / length};
}

} // namespace Pt