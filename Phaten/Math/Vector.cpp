#include "Vector.hpp"

#include <string>

namespace Pt {

const Vector2 Vector2::ZERO = Vector2{0, 0};
const Vector2 Vector2::ONE = Vector2{1, 1};
const Vector2 Vector2::X = Vector2{1, 0};
const Vector2 Vector2::Y = Vector2{0, 1};

bool Vector2::FromString(std::string_view str)
{
    size_t space = str.find(' ');
    if (space == std::string::npos) return false;

    x = std::stof(std::string{str.substr(0, space)});
    y = std::stof(std::string{str.substr(space + 1)});
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
    return std::to_string(x) + " " + std::to_string(y);
}

const Vector3 Vector3::ZERO = Vector3{0, 0, 0};
const Vector3 Vector3::ONE = Vector3{1, 1, 1};
const Vector3 Vector3::X = Vector3{1, 0, 0};
const Vector3 Vector3::Y = Vector3{0, 1, 0};
const Vector3 Vector3::Z = Vector3{0, 0, 1};

Vector3::Vector3(const Vector4& vector) :
    x(vector.x),
    y(vector.y),
    z(vector.z)
{
}

bool Vector3::FromString(std::string_view str)
{
    size_t space1 = str.find(' ');
    if (space1 == std::string::npos) return false;

    size_t space2 = str.find(' ', space1 + 1);
    if (space2 == std::string::npos) return false; 

    x = std::stof(std::string{str.substr(0, space1)});
    y = std::stof(std::string{str.substr(space1 + 1, space2 - space1 - 1)});
    z = std::stof(std::string{str.substr(space2 + 1)});
    return true;
}

std::string Vector3::ToString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

Vector3 operator * (float lhs, const Vector3& rhs)
{
    return rhs * lhs;
}

const Vector4 Vector4::ZERO = Vector4{0, 0, 0, 0};
const Vector4 Vector4::ONE = Vector4{1, 1, 1, 1};
const Vector4 Vector4::X = Vector4{1, 0, 0, 0};
const Vector4 Vector4::Y = Vector4{0, 1, 0, 0};
const Vector4 Vector4::Z = Vector4{0, 0, 1, 0};
const Vector4 Vector4::W = Vector4{0, 0, 0, 1};

bool Vector4::FromString(std::string_view str)
{
    size_t space1 = str.find(' ');
    if (space1 == std::string::npos) return false;

    size_t space2 = str.find(' ', space1 + 1);
    if (space2 == std::string::npos) return false; 

    size_t space3 = str.find(' ', space2 + 1);
    if (space3 == std::string::npos) return false; 

    x = std::stof(std::string{str.substr(0, space1)});
    y = std::stof(std::string{str.substr(space1 + 1, space2 - space1 - 1)});
    z = std::stof(std::string{str.substr(space2 + 1, space3 - space2 - 1)});
    w = std::stof(std::string{str.substr(space3 + 1)});
    return true;
}

std::string Vector4::ToString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + " " + std::to_string(w);
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