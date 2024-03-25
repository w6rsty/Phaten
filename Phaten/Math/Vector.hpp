#pragma once

#include <string_view>

#include "Math.hpp"
#include "IO/Assert.hpp"

namespace Pt {

class Vector2;
class Vector3;
class Vector4;

class Vector2
{
public:
    union
    {
        struct { float x, y; };
        struct { float u, v; };
        float data[2];
    };

    Vector2()
    {
        PT_LOG_WARN("Uninitialized Vector2 object.");
    }

    Vector2(float value) :
        x(value),
        y(value)
    {
    }

    Vector2(const Vector2& vector) :
        x(vector.x),
        y(vector.y)
    {
    }

    Vector2(float x_, float y_) :
        x(x_),
        y(y_)
    {
    }

    Vector2(const float* data) :
        x(data[0]),
        y(data[1])
    {
    }

    Vector2(std::string_view str)
    {
        if (!FromString(str)) Vector2();
    }

    Vector2& operator = (const Vector2& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        return *this;
    }

    Vector2& operator += (const Vector2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2& operator -= (const Vector2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vector2& operator * (float rhs)
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }

    Vector2& operator / (float rhs)
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    bool operator == (const Vector2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator != (const Vector2& rhs) const { return x != rhs.x || y != rhs.y; }
    Vector2 operator + (const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
    Vector2 operator - () const { return Vector2(-x, -y); }
    Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 operator * (float rhs) const { return Vector2(x * rhs, y * rhs); }
    Vector2 operator / (float rhs) const { return Vector2(x / rhs, y / rhs); }
    
    float& operator [] (size_t index) { return data[index]; }
    const float& operator [] (size_t index) const { return data[index]; }

    float& At(size_t index) { PT_ASSERT(index < 2); return data[index];}
    const float& At(size_t index) const { PT_ASSERT(index < 2); return data[index]; }

    std::string ToString() const;
    const float* Data() const { return data; }

    void Normalize()
    {
        float lengthSquared = LengthSquared();
        if (!::Equal(lengthSquared, 1.0f) && lengthSquared > 0.0f)
        {
            float invLength = 1.0f / sqrtf(lengthSquared);
            x *= invLength;
            y *= invLength;
        }
    }
    Vector2 Normalized() const
    {
        Vector2 result = *this;
        result.Normalize();
        return result;
    }
    float LengthSquared() const { return x * x + y * y; }
    float Length() const { return sqrtf(LengthSquared()); }
    float Dot(const Vector2& rhs) const { return x * rhs.x + y * rhs.y; }
    float Cross(const Vector2& rhs) const { return x * rhs.y - y * rhs.x; }

    Vector2 Abs() const { return Vector2(::Abs(x), ::Abs(y)); }
    Vector2 LinearLerp(const Vector2& rhs, float t) const { return *this + (rhs - *this) * t; }
    bool Equal(const Vector2& rhs, float epsilon = ::M_EPSILON) const { return ::Equal(x, rhs.x, epsilon) && ::Equal(y, rhs.y, epsilon); }
    bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y); }

    const static Vector2 Zero;
    const static Vector2 One;
    const static Vector2 X;
    const static Vector2 Y;
private:
    bool FromString(std::string_view str);
};

class Vector3
{
public:
    union
    {
        struct { float x, y, z; };
        struct { float r, g, b; };
        float data[3];
    };

    Vector3()
    {
        PT_LOG_WARN("Uninitialized Vector3 object.");
    }

    Vector3(float value) :
        x(value),
        y(value),
        z(value)
    {
    }

    Vector3(const Vector3& vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z)
    {
    }

    Vector3(float x_, float y_, float z_) :
        x(x_),
        y(y_),
        z(z_)
    {
    }

    Vector3(const float* data) :
        x(data[0]),
        y(data[1]),
        z(data[2])
    {
    }

    Vector3(std::string_view str)
    {
        if (!FromString(str)) Vector3();
    }

    Vector3& operator = (const Vector3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    Vector3& operator += (const Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vector3& operator -= (const Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vector3& operator * (float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    Vector3& operator / (float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    bool operator == (const Vector3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
    bool operator != (const Vector3& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z; }
    Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 operator - () const { return Vector3(-x, -y, -z); }
    Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator * (float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }

    float& operator [] (size_t index) { return data[index]; }
    const float& operator [] (size_t index) const { return data[index]; }

    float& At(size_t index) { PT_ASSERT(index < 3); return data[index];}
    const float& At(size_t index) const { PT_ASSERT(index < 3); return data[index]; }

    std::string ToString() const;
    const float* Data() const { return data; }

    void Normalize()
    {
        float lengthSquared = LengthSquared();
        if (!::Equal(lengthSquared, 1.0f) && lengthSquared > 0.0f)
        {
            float invLength = 1.0f / sqrtf(lengthSquared);
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
    }
    Vector3 Normalized() const
    {
        Vector3 result = *this;
        result.Normalize();
        return result;
    }
    float LengthSquared() const { return x * x + y * y + z * z; }
    float Length() const { return sqrtf(LengthSquared()); }
    float Dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }
    Vector3 Cross(const Vector3& rhs) const { return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x); }

    Vector3 Abs() const { return Vector3(::Abs(x), ::Abs(y), ::Abs(z)); }
    Vector3 LinearLerp(const Vector3& rhs, float t) const { return *this + (rhs - *this) * t; }
    bool Equal(const Vector3& rhs, float epsilon = ::M_EPSILON) const { return ::Equal(x, rhs.x, epsilon) && ::Equal(y, rhs.y, epsilon) && ::Equal(z, rhs.z, epsilon); }
    bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z); }

    const static Vector3 Zero;
    const static Vector3 One;
    const static Vector3 X;
    const static Vector3 Y;
    const static Vector3 Z;
private:
    bool FromString(std::string_view str);
};

class Vector4
{
public:
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        float data[4];
    };

    Vector4()
    {
        PT_LOG_WARN("Uninitialized Vector4 object.");
    }

    Vector4(float value) :
        x(value),
        y(value),
        z(value),
        w(value)
    {
    }

    Vector4(const Vector4& vector) :
        x(vector.x),
        y(vector.y),
        z(vector.z),
        w(vector.w)
    {
    }

    Vector4(float x_, float y_, float z_, float w_) :
        x(x_),
        y(y_),
        z(z_),
        w(w_)
    {
    }

    Vector4(const float* data) :
        x(data[0]),
        y(data[1]),
        z(data[2]),
        w(data[3])
    {
    }

    Vector4(std::string_view str)
    {
        if (!FromString(str)) Vector4();
    }

    Vector4& operator = (const Vector4& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        w = rhs.w;
        return *this;
    }

    Vector4& operator += (const Vector4& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }

    Vector4& operator -= (const Vector4& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }

    Vector4& operator * (float rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }

    Vector4& operator / (float rhs)
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    bool operator == (const Vector4& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w; }
    bool operator != (const Vector4& rhs) const { return x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w; }
    Vector4 operator + (const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); 
    }
    Vector4 operator - () const { return Vector4(-x, -y, -z, -w); }
    Vector4 operator - (const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
    Vector4 operator * (float rhs) const { return Vector4(x * rhs, y * rhs, z * rhs, w * rhs); }

    float& operator [] (size_t index) { return data[index]; }
    const float& operator [] (size_t index) const { return data[index]; }

    float& At(size_t index) { PT_ASSERT(index < 4); return data[index];}
    const float& At(size_t index) const { PT_ASSERT(index < 4); return data[index]; }

    std::string ToString() const;
    const float* Data() const { return data; }

    void Normalize()
    {
        float lengthSquared = LengthSquared();
        if (!::Equal(lengthSquared, 1.0f) && lengthSquared > 0.0f)
        {
            float invLength = 1.0f / sqrtf(lengthSquared);
            x *= invLength;
            y *= invLength;
            z *= invLength;
            w *= invLength;
        }
    }
    Vector4 Normalized() const
    {
        Vector4 result = *this;
        result.Normalize();
        return result;
    }
    float LengthSquared() const { return x * x + y * y + z * z + w * w; }
    float Length() const { return sqrtf(LengthSquared()); }
    float Dot(const Vector4& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w; }

    Vector4 Abs() const { return Vector4(::Abs(x), ::Abs(y), ::Abs(z), ::Abs(w)); }
    Vector4 LinearLerp(const Vector4& rhs, float t) const { return *this + (rhs - *this) * t; }
    bool Equal(const Vector4& rhs, float epsilon = ::M_EPSILON) const { return ::Equal(x, rhs.x, epsilon) && ::Equal(y, rhs.y, epsilon) && ::Equal(z, rhs.z, epsilon) && ::Equal(w, rhs.w, epsilon); }
    bool IsNaN() const { return ::IsNaN(x) || ::IsNaN(y) || ::IsNaN(z) || ::IsNaN(w); }

    const static Vector4 Zero;
    const static Vector4 One;
    const static Vector4 X;
    const static Vector4 Y;
    const static Vector4 Z;
    const static Vector4 W;
private:
    bool FromString(std::string_view str);
};

} // namespace Pt