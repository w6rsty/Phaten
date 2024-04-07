#pragma once

#include <cstdint>
#include <string_view>

#include "IO/Assert.hpp"

namespace Pt {

class Vector2;
class Vector3;

class IntV2
{
public:
    union {
        struct { int32_t x, y; };
        int32_t data[2];
    };

    IntV2()
    {
    }

    IntV2(const IntV2& vector) :
        x(vector.x),
        y(vector.y)
    {
    }

    IntV2(int32_t x_, int32_t y_) :
        x(x_),
        y(y_)
    {
    }

    IntV2(const int32_t* data) :
        x(data[0]),
        y(data[1])
    {
    }

    IntV2(std::string_view str)
    {
        FromString(str);
    }

    IntV2(const Vector2& vector);

    IntV2& operator = (const IntV2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }


    IntV2& operator += (const IntV2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    IntV2& operator -= (const IntV2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    IntV2& operator * (int32_t rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    IntV2& operator / (int32_t rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    bool operator == (const IntV2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator != (const IntV2& rhs) const { return !(*this == rhs); }
    IntV2 operator + (const IntV2& rhs) const { return IntV2{x + rhs.x, y + rhs.y}; }
    IntV2 operator - () const { return IntV2{-x, -y}; }
    IntV2 operator - (const IntV2& rhs) const { return IntV2{x - rhs.x, y - rhs.y}; }
    IntV2 operator * (int32_t rhs) const { return IntV2{x * rhs, y * rhs}; }
    IntV2 operator / (int32_t rhs) const { return IntV2{x / rhs, y / rhs}; }

    int32_t& operator [] (int32_t index) { return data[index]; }
    const int32_t& operator [] (int32_t index) const { return data[index]; }

    int32_t& At(size_t index) { PT_ASSERT(index < 2); return data[index]; }
    const int32_t& At(size_t index) const { PT_ASSERT(index < 2); return data[index]; }

    std::string ToString() const;
    const int32_t* Data() const { return data; }

    static const IntV2 ZERO;
    static const IntV2 ONE;
    static const IntV2 X;
    static const IntV2 Y;
private:
    bool FromString(std::string_view str);
};

class IntV3
{
public:
    union {
        struct { int32_t x, y, z; };
        struct { int32_t r, g, b; };
        int32_t data[3];
    };

    IntV3()
    {
    }

    IntV3(const IntV3& vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z)
    {
    }

    IntV3(int32_t x_, int32_t y_, int32_t z_) :
        x(x_),
        y(y_),
        z(z_)
    {
    }

    IntV3(const int32_t* data) :
        x(data[0]),
        y(data[1]),
        z(data[2])
    {
    }

    IntV3(std::string_view str)
    {
        FromString(str);
    }

    IntV3(const IntV2& v2, int32_t z_) :
        x(v2.x),
        y(v2.y),
        z(z_)
    {
    }

    IntV3(const Vector3& vector);

    IntV3& operator += (const IntV3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    IntV3& operator -= (const IntV3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    IntV3& operator * (int32_t rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    IntV3& operator / (int32_t rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    bool operator == (const IntV3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator != (const IntV3& rhs) const { return !(*this == rhs); }
    IntV3 operator + (const IntV3& rhs) const { return IntV3{x + rhs.x, y + rhs.y, z + rhs.z}; }
    IntV3 operator - () const { return IntV3{-x, -y, -z}; }
    IntV3 operator - (const IntV3& rhs) const { return IntV3{x - rhs.x, y - rhs.y, z - rhs.z}; }
    IntV3 operator * (int32_t rhs) const { return IntV3{x * rhs, y * rhs, z * rhs}; }
    IntV3 operator / (int32_t rhs) const { return IntV3{x / rhs, y / rhs, z / rhs}; }

    int32_t& operator [] (size_t index) { return data[index]; }
    const int32_t& operator [] (size_t index) const { return data[index]; }

    int32_t& At(int32_t index) { PT_ASSERT(index < 3); return data[index]; }
    const int32_t& At(int32_t index) const { PT_ASSERT(index < 3); return data[index]; }

    std::string ToString() const;
    const int32_t* Data() const { return data; }

    static const IntV3 ZERO;
    static const IntV3 ONE;
    static const IntV3 X;
    static const IntV3 Y;
    static const IntV3 Z;
private:
    bool FromString(std::string_view str);
};

} // namespace Pt