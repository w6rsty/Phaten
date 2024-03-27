#pragma once

#include <string>
#include <string_view>

#include "Math.hpp"
#include "Vector.hpp"

namespace Pt {

// Column-major matrix

class Matrix3;
class Matrix4;

class Matrix3
{
public:
    float data[3][3];

    Matrix3()
    {
    }

    // Copy constructor
    Matrix3(const Matrix3& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];
    }

    // Construct from 3 vectors
    Matrix3(const Vector3& v1, const Vector3& v2, const Vector3& v3)
    {
        this->data[0][0] = v1.x;
        this->data[1][0] = v1.y;
        this->data[2][0] = v1.z;

        this->data[0][1] = v2.x;
        this->data[1][1] = v2.y;
        this->data[2][1] = v2.z;

        this->data[0][2] = v3.x;
        this->data[1][2] = v3.y;
        this->data[2][2] = v3.z;
    }

    Matrix3(const float* data)
    {
        this->data[0][0] = data[0];
        this->data[1][0] = data[1];
        this->data[2][0] = data[2];

        this->data[0][1] = data[3];
        this->data[1][1] = data[4];
        this->data[2][1] = data[5];

        this->data[0][2] = data[6];
        this->data[1][2] = data[7];
        this->data[2][2] = data[8];
    }

    Matrix3(std::string_view str)
    {
        if (!FromString(str))
            Matrix3(); // use default constructor if failed
    }

    Matrix3(const Matrix4& matrix);

    Matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22)
    {
        this->data[0][0] = m00;
        this->data[1][0] = m10;
        this->data[2][0] = m20;

        this->data[0][1] = m01;
        this->data[1][1] = m11;
        this->data[2][1] = m21;

        this->data[0][2] = m02;
        this->data[1][2] = m12;
        this->data[2][2] = m22;
    }

    Matrix3& operator = (const Matrix3& rhs)
    {
        this->data[0][0] = rhs.data[0][0];
        this->data[1][0] = rhs.data[1][0];
        this->data[2][0] = rhs.data[2][0];

        this->data[0][1] = rhs.data[0][1];
        this->data[1][1] = rhs.data[1][1];
        this->data[2][1] = rhs.data[2][1];

        this->data[0][2] = rhs.data[0][2];
        this->data[1][2] = rhs.data[1][2];
        this->data[2][2] = rhs.data[2][2];

        return *this;
    }

    bool operator == (const Matrix3& rhs) const 
    {
        const float* lhsData = &data[0][0];
        const float* rhsData = &rhs.data[0][0];

        for (int i = 0; i < 9; i++)
        {
            if (lhsData[i] != rhsData[i])
                return false;
        }

        return true;
    }

    bool operator != (const Matrix3& rhs) const { return !(*this == rhs); }

    Vector3 operator * (const Vector3& rhs) const
    {
        return Vector3(
            data[0][0] * rhs.x + data[1][0] * rhs.y + data[2][0] * rhs.z,
            data[0][1] * rhs.x + data[1][1] * rhs.y + data[2][1] * rhs.z,
            data[0][2] * rhs.x + data[1][2] * rhs.y + data[2][2] * rhs.z
        );
    }

    Matrix3 operator + (const Matrix3& rhs) const
    {
        return Matrix3(
            Vector3(data[0][0] + rhs.data[0][0], data[1][0] + rhs.data[1][0], data[2][0] + rhs.data[2][0]),
            Vector3(data[0][1] + rhs.data[0][1], data[1][1] + rhs.data[1][1], data[2][1] + rhs.data[2][1]),
            Vector3(data[0][2] + rhs.data[0][2], data[1][2] + rhs.data[1][2], data[2][2] + rhs.data[2][2])
        );
    }

    Matrix3 operator - (const Matrix3& rhs) const
    {
        return Matrix3(
            Vector3(data[0][0] - rhs.data[0][0], data[1][0] - rhs.data[1][0], data[2][0] - rhs.data[2][0]),
            Vector3(data[0][1] - rhs.data[0][1], data[1][1] - rhs.data[1][1], data[2][1] - rhs.data[2][1]),
            Vector3(data[0][2] - rhs.data[0][2], data[1][2] - rhs.data[1][2], data[2][2] - rhs.data[2][2])
        );
    }

    Matrix3 operator * (float rhs) const
    {
        return Matrix3(
            Vector3(data[0][0] * rhs, data[1][0] * rhs, data[2][0] * rhs),
            Vector3(data[0][1] * rhs, data[1][1] * rhs, data[2][1] * rhs),
            Vector3(data[0][2] * rhs, data[1][2] * rhs, data[2][2] * rhs)
        );
    }

    Matrix3 operator * (const Matrix3& rhs) const
    {
        return Matrix3(
            Vector3(
                data[0][0] * rhs.data[0][0] + data[1][0] * rhs.data[0][1] + data[2][0] * rhs.data[0][2],
                data[0][0] * rhs.data[1][0] + data[1][0] * rhs.data[1][1] + data[2][0] * rhs.data[1][2],
                data[0][0] * rhs.data[2][0] + data[1][0] * rhs.data[2][1] + data[2][0] * rhs.data[2][2]
            ),
            Vector3(
                data[0][1] * rhs.data[0][0] + data[1][1] * rhs.data[0][1] + data[2][1] * rhs.data[0][2],
                data[0][1] * rhs.data[1][0] + data[1][1] * rhs.data[1][1] + data[2][1] * rhs.data[1][2],
                data[0][1] * rhs.data[2][0] + data[1][1] * rhs.data[2][1] + data[2][1] * rhs.data[2][2]
            ),
            Vector3(
                data[0][2] * rhs.data[0][0] + data[1][2] * rhs.data[0][1] + data[2][2] * rhs.data[0][2],
                data[0][2] * rhs.data[1][0] + data[1][2] * rhs.data[1][1] + data[2][2] * rhs.data[1][2],
                data[0][2] * rhs.data[2][0] + data[1][2] * rhs.data[2][1] + data[2][2] * rhs.data[2][2]
            )
        );
    }

    float* operator [] (int column) { return data[column]; }
    const float* operator [] (int column) const { return data[column]; }

    Vector3 Column(int column) const
    {
        return Vector3(data[0][column], data[1][column], data[2][column]);
    }

    Vector3 Row(int row) const
    {
        return Vector3(data[row][0], data[row][1], data[row][2]);
    }

    Matrix3 FromRows(const Vector3& row1, const Vector3& row2, const Vector3& row3)
    {
        return Matrix3(
            Vector3(row1.x, row2.x, row3.x),
            Vector3(row1.y, row2.y, row3.y),
            Vector3(row1.z, row2.z, row3.z)
        );
    }

    bool Equals(const Matrix3& rhs, float epsilon = M_EPSILON) const
    {
        const float* lhsData = &data[0][0];
        const float* rhsData = &rhs.data[0][0];

        for (int i = 0; i < 9; i++)
        {
            if (!::EpsilonEqual(lhsData[i], rhsData[i], epsilon))
                return false;
        }

        return true;
    }

    const float* Data() const
    {
        return &data[0][0];
    }

    std::string ToString() const;

    Matrix3 Transpose() const
    {
        return Matrix3(
            Vector3(data[0][0], data[0][1], data[0][2]),
            Vector3(data[1][0], data[1][1], data[1][2]),
            Vector3(data[2][0], data[2][1], data[2][2])
        );
    }

    void Transpose()
    {
        std::swap(data[0][1], data[1][0]);
        std::swap(data[0][2], data[2][0]);
        std::swap(data[1][2], data[2][1]);
    }

    float Determinant() const
    {
        return data[0][0] * (data[1][1] * data[2][2] - data[1][2] * data[2][1]) -
               data[0][1] * (data[1][0] * data[2][2] - data[1][2] * data[2][0]) +
               data[0][2] * (data[1][0] * data[2][1] - data[1][1] * data[2][0]);
    }

    Matrix3 Inverse() const
    {
        float det = Determinant();
        if (det == 0.0f)
            return Matrix3::ZERO;

        float invDet = 1.0f / det;
        return Matrix3(
            Vector3(
                (data[1][1] * data[2][2] - data[1][2] * data[2][1]) * invDet,
                (data[0][2] * data[2][1] - data[0][1] * data[2][2]) * invDet,
                (data[0][1] * data[1][2] - data[0][2] * data[1][1]) * invDet
            ),
            Vector3(
                (data[1][2] * data[2][0] - data[1][0] * data[2][2]) * invDet,
                (data[0][0] * data[2][2] - data[0][2] * data[2][0]) * invDet,
                (data[0][2] * data[1][0] - data[0][0] * data[1][2]) * invDet
            ),
            Vector3(
                (data[1][0] * data[2][1] - data[1][1] * data[2][0]) * invDet,
                (data[0][1] * data[2][0] - data[0][0] * data[2][1]) * invDet,
                (data[0][0] * data[1][1] - data[0][1] * data[1][0]) * invDet
            )
        );
    }

    static const Matrix3 ZERO;
    static const Matrix3 IDENTITY;
private:
    bool FromString(std::string_view str);
};

class Matrix4
{
public:
    float data[4][4];

    Matrix4()
    {
    }

    // Copy constructor
    Matrix4(const Matrix4& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];
        this->data[3][0] = matrix.data[3][0];

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];
        this->data[3][1] = matrix.data[3][1];

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];
        this->data[3][2] = matrix.data[3][2];

        this->data[0][3] = matrix.data[0][3];
        this->data[1][3] = matrix.data[1][3];
        this->data[2][3] = matrix.data[2][3];
        this->data[3][3] = matrix.data[3][3];
    }

    // Construct from 4 vectors
    Matrix4(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4)
    {
        this->data[0][0] = v1.x;
        this->data[1][0] = v1.y;
        this->data[2][0] = v1.z;
        this->data[3][0] = v1.w;

        this->data[0][1] = v2.x;
        this->data[1][1] = v2.y;
        this->data[2][1] = v2.z;
        this->data[3][1] = v2.w;

        this->data[0][2] = v3.x;
        this->data[1][2] = v3.y;
        this->data[2][2] = v3.z;
        this->data[3][2] = v3.w;

        this->data[0][3] = v4.x;
        this->data[1][3] = v4.y;
        this->data[2][3] = v4.z;
        this->data[3][3] = v4.w;
    }

    Matrix4(const float* data)
    {
        this->data[0][0] = data[0];
        this->data[1][0] = data[1];
        this->data[2][0] = data[2];
        this->data[3][0] = data[3];

        this->data[0][1] = data[4];
        this->data[1][1] = data[5];
        this->data[2][1] = data[6];
        this->data[3][1] = data[7];

        this->data[0][2] = data[8];
        this->data[1][2] = data[9];
        this->data[2][2] = data[10];
        this->data[3][2] = data[11];

        this->data[0][3] = data[12];
        this->data[1][3] = data[13];
        this->data[2][3] = data[14];
        this->data[3][3] = data[15];
    }

    Matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33)
    {
        this->data[0][0] = m00;
        this->data[1][0] = m10;
        this->data[2][0] = m20;
        this->data[3][0] = m30;

        this->data[0][1] = m01;
        this->data[1][1] = m11;
        this->data[2][1] = m21;
        this->data[3][1] = m31;

        this->data[0][2] = m02;
        this->data[1][2] = m12;
        this->data[2][2] = m22;
        this->data[3][2] = m32;

        this->data[0][3] = m03;
        this->data[1][3] = m13;
        this->data[2][3] = m23;
        this->data[3][3] = m33;
    }

    Matrix4(std::string_view str)
    {
        if(FromString(str))
            Matrix4(); // use default constructor if failed
    }

    Matrix4(const Matrix3& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];
        this->data[3][0] = 0.0f;

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];
        this->data[3][1] = 0.0f;

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];
        this->data[3][2] = 0.0f;

        this->data[0][3] = 0.0f;
        this->data[1][3] = 0.0f;
        this->data[2][3] = 0.0f;
        this->data[3][3] = 1.0f;
    }

    Matrix4& operator = (const Matrix4& rhs)
    {
        this->data[0][0] = rhs.data[0][0];
        this->data[1][0] = rhs.data[1][0];
        this->data[2][0] = rhs.data[2][0];
        this->data[3][0] = rhs.data[3][0];

        this->data[0][1] = rhs.data[0][1];
        this->data[1][1] = rhs.data[1][1];
        this->data[2][1] = rhs.data[2][1];
        this->data[3][1] = rhs.data[3][1];

        this->data[0][2] = rhs.data[0][2];
        this->data[1][2] = rhs.data[1][2];
        this->data[2][2] = rhs.data[2][2];
        this->data[3][2] = rhs.data[3][2];

        this->data[0][3] = rhs.data[0][3];
        this->data[1][3] = rhs.data[1][3];
        this->data[2][3] = rhs.data[2][3];
        this->data[3][3] = rhs.data[3][3];

        return *this;
    }

    bool operator == (const Matrix4& rhs) const 
    {
        const float* lhsData = &data[0][0];
        const float* rhsData = &rhs.data[0][0];

        for (int i = 0; i < 16; i++)
        {
            if (lhsData[i] != rhsData[i])
                return false;
        }

        return true;
    }

    bool operator != (const Matrix4& rhs) const { return !(*this == rhs); }

    Vector4 operator * (const Vector4& rhs) const
    {
        return Vector4(
            data[0][0] * rhs.x + data[1][0] * rhs.y + data[2][0] * rhs.z + data[3][0] * rhs.w,
            data[0][1] * rhs.x + data[1][1] * rhs.y + data[2][1] * rhs.z + data[3][1] * rhs.w,
            data[0][2] * rhs.x + data[1][2] * rhs.y + data[2][2] * rhs.z + data[3][2] * rhs.w,
            data[0][3] * rhs.x + data[1][3] * rhs.y + data[2][3] * rhs.z + data[3][3] * rhs.w
        );
    }

    Matrix4 operator + (const Matrix4& rhs) const
    {
        return Matrix4(
            Vector4(data[0][0] + rhs.data[0][0], data[1][0] + rhs.data[1][0], data[2][0] + rhs.data[2][0], data[3][0] + rhs.data[3][0]),
            Vector4(data[0][1] + rhs.data[0][1], data[1][1] + rhs.data[1][1], data[2][1] + rhs.data[2][1], data[3][1] + rhs.data[3][1]),
            Vector4(data[0][2] + rhs.data[0][2], data[1][2] + rhs.data[1][2], data[2][2] + rhs.data[2][2], data[3][2] + rhs.data[3][2]),
            Vector4(data[0][3] + rhs.data[0][3], data[1][3] + rhs.data[1][3], data[2][3] + rhs.data[2][3], data[3][3] + rhs.data[3][3])
        );
    }

    Matrix4 operator - (const Matrix4& rhs) const
    {
        return Matrix4(
            Vector4(data[0][0] - rhs.data[0][0], data[1][0] - rhs.data[1][0], data[2][0] - rhs.data[2][0], data[3][0] - rhs.data[3][0]),
            Vector4(data[0][1] - rhs.data[0][1], data[1][1] - rhs.data[1][1], data[2][1] - rhs.data[2][1], data[3][1] - rhs.data[3][1]),
            Vector4(data[0][2] - rhs.data[0][2], data[1][2] - rhs.data[1][2], data[2][2] - rhs.data[2][2], data[3][2] - rhs.data[3][2]),
            Vector4(data[0][3] - rhs.data[0][3], data[1][3] - rhs.data[1][3], data[2][3] - rhs.data[2][3], data[3][3] - rhs.data[3][3])
        );
    }

    Matrix4 operator * (float rhs) const
    {
        return Matrix4(
            Vector4(data[0][0] * rhs, data[1][0] * rhs, data[2][0] * rhs, data[3][0] * rhs),
            Vector4(data[0][1] * rhs, data[1][1] * rhs, data[2][1] * rhs, data[3][1] * rhs),
            Vector4(data[0][2] * rhs, data[1][2] * rhs, data[2][2] * rhs, data[3][2] * rhs),
            Vector4(data[0][3] * rhs, data[1][3] * rhs, data[2][3] * rhs, data[3][3] * rhs)
        );
    }

    Matrix4 operator * (const Matrix4& rhs) const
    {
        return Matrix4(
            Vector4(
                data[0][0] * rhs.data[0][0] + data[1][0] * rhs.data[0][1] + data[2][0] * rhs.data[0][2] + data[3][0] * rhs.data[0][3],
                data[0][0] * rhs.data[1][0] + data[1][0] * rhs.data[1][1] + data[2][0] * rhs.data[1][2] + data[3][0] * rhs.data[1][3],
                data[0][0] * rhs.data[2][0] + data[1][0] * rhs.data[2][1] + data[2][0] * rhs.data[2][2] + data[3][0] * rhs.data[2][3],
                data[0][0] * rhs.data[3][0] + data[1][0] * rhs.data[3][1] + data[2][0] * rhs.data[3][2] + data[3][0] * rhs.data[3][3]
            ),
            Vector4(
                data[0][1] * rhs.data[0][0] + data[1][1] * rhs.data[0][1] + data[2][1] * rhs.data[0][2] + data[3][1] * rhs.data[0][3],
                data[0][1] * rhs.data[1][0] + data[1][1] * rhs.data[1][1] + data[2][1] * rhs.data[1][2] + data[3][1] * rhs.data[1][3],
                data[0][1] * rhs.data[2][0] + data[1][1] * rhs.data[2][1] + data[2][1] * rhs.data[2][2] + data[3][1] * rhs.data[2][3],
                data[0][1] * rhs.data[3][0] + data[1][1] * rhs.data[3][1] + data[2][1] * rhs.data[3][2] + data[3][1] * rhs.data[3][3]
            ),
            Vector4(
                data[0][2] * rhs.data[0][0] + data[1][2] * rhs.data[0][1] + data[2][2] * rhs.data[0][2] + data[3][2] * rhs.data[0][3],
                data[0][2] * rhs.data[1][0] + data[1][2] * rhs.data[1][1] + data[2][2] * rhs.data[1][2] + data[3][2] * rhs.data[1][3],
                data[0][2] * rhs.data[2][0] + data[1][2] * rhs.data[2][1] + data[2][2] * rhs.data[2][2] + data[3][2] * rhs.data[2][3],
                data[0][2] * rhs.data[3][0] + data[1][2] * rhs.data[3][1] + data[2][2] * rhs.data[3][2] + data[3][2] * rhs.data[3][3]
            ),
            Vector4(
                data[0][3] * rhs.data[0][0] + data[1][3] * rhs.data[0][1] + data[2][3] * rhs.data[0][2] + data[3][3] * rhs.data[0][3],
                data[0][3] * rhs.data[1][0] + data[1][3] * rhs.data[1][1] + data[2][3] * rhs.data[1][2] + data[3][3] * rhs.data[1][3],
                data[0][3] * rhs.data[2][0] + data[1][3] * rhs.data[2][1] + data[2][3] * rhs.data[2][2] + data[3][3] * rhs.data[2][3],
                data[0][3] * rhs.data[3][0] + data[1][3] * rhs.data[3][1] + data[2][3] * rhs.data[3][2] + data[3][3] * rhs.data[3][3]
            )
        );
    }

    float* operator [] (int column) { return data[column]; }
    const float* operator [] (int column) const { return data[column]; }

    Vector4 Column(int column) const
    {
        return Vector4(data[0][column], data[1][column], data[2][column], data[3][column]);
    }

    Vector4 Row(int row) const
    {
        return Vector4(data[row][0], data[row][1], data[row][2], data[row][3]);
    }

    Matrix4 FromRows(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4)
    {
        return Matrix4(
            Vector4(row1.x, row2.x, row3.x, row4.x),
            Vector4(row1.y, row2.y, row3.y, row4.y),
            Vector4(row1.z, row2.z, row3.z, row4.z),
            Vector4(row1.w, row2.w, row3.w, row4.w)
        );
    }

    bool Equals(const Matrix4& rhs, float epsilon = M_EPSILON) const
    {
        const float* lhsData = &data[0][0];
        const float* rhsData = &rhs.data[0][0];

        for (int i = 0; i < 16; i++)
        {
            if (!::EpsilonEqual(lhsData[i], rhsData[i], epsilon))
                return false;
        }

        return true;
    }

    const float* Data() const
    {
        return &data[0][0];
    }

    std::string ToString() const;

    Matrix4 Transpose() const
    {
        return Matrix4(
            Vector4(data[0][0], data[0][1], data[0][2], data[0][3]),
            Vector4(data[1][0], data[1][1], data[1][2], data[1][3]),
            Vector4(data[2][0], data[2][1], data[2][2], data[2][3]),
            Vector4(data[3][0], data[3][1], data[3][2], data[3][3])
        );
    }

    void Transpose()
    {
        std::swap(data[0][1], data[1][0]);
        std::swap(data[0][2], data[2][0]);
        std::swap(data[0][3], data[3][0]);
        std::swap(data[1][2], data[2][1]);
        std::swap(data[1][3], data[3][1]);
        std::swap(data[2][3], data[3][2]);
    }

    float Determinant() const
    {
        return data[0][0] * (data[1][1] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) -
                             data[1][2] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) +
                             data[1][3] * (data[2][1] * data[3][2] - data[2][2] * data[3][1])) -
               data[0][1] * (data[1][0] * (data[2][2] * data[3][3] - data[2][3] * data[3][2]) -
                             data[1][2] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) +
                             data[1][3] * (data[2][0] * data[3][2] - data[2][2] * data[3][0])) +
               data[0][2] * (data[1][0] * (data[2][1] * data[3][3] - data[2][3] * data[3][1]) -
                             data[1][1] * (data[2][0] * data[3][3] - data[2][3] * data[3][0]) +
                             data[1][3] * (data[2][0] * data[3][1] - data[2][1] * data[3][0])) -
               data[0][3] * (data[1][0] * (data[2][1] * data[3][2] - data[2][2] * data[3][1]) -
                             data[1][1] * (data[2][0] * data[3][2] - data[2][2] * data[3][0]) +
                             data[1][2] * (data[2][0] * data[3][1] - data[2][1] * data[3][0]));
    }
    
    // TODO: Implement Inverse() function
    // Matrix4 Inverse() const;

    static const Matrix4 ZERO;
    static const Matrix4 IDENTITY;
private:
    bool FromString(std::string_view str);
};

/// 3x4 matrix for scene node transform calculations.
class Matrix3x4
{
public:
    float data[3][4];

    Matrix3x4()
    {
    }

    /// Copy constructor.
    Matrix3x4(const Matrix3x4& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];

        this->data[0][3] = matrix.data[0][3];
        this->data[1][3] = matrix.data[1][3];
        this->data[2][3] = matrix.data[2][3];
    }

    /// Construct from a matrix3.
    Matrix3x4(const Matrix3& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];

        this->data[0][3] = 0.0f;
        this->data[1][3] = 0.0f;
        this->data[2][3] = 0.0f;
    }

    /// Construct from a matrix4.
    Matrix3x4(const Matrix4& matrix)
    {
        this->data[0][0] = matrix.data[0][0];
        this->data[1][0] = matrix.data[1][0];
        this->data[2][0] = matrix.data[2][0];

        this->data[0][1] = matrix.data[0][1];
        this->data[1][1] = matrix.data[1][1];
        this->data[2][1] = matrix.data[2][1];

        this->data[0][2] = matrix.data[0][2];
        this->data[1][2] = matrix.data[1][2];
        this->data[2][2] = matrix.data[2][2];

        this->data[0][3] = matrix.data[0][3];
        this->data[1][3] = matrix.data[1][3];
        this->data[2][3] = matrix.data[2][3];
    }

    /// Construct from 3 vectors.
    Matrix3x4(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4)
    {
        this->data[0][0] = v1.x;
        this->data[1][0] = v1.y;
        this->data[2][0] = v1.z;

        this->data[0][1] = v2.x;
        this->data[1][1] = v2.y;
        this->data[2][1] = v2.z;

        this->data[0][2] = v3.x;
        this->data[1][2] = v3.y;
        this->data[2][2] = v3.z;

        this->data[0][3] = v4.x;
        this->data[1][3] = v4.y;
        this->data[2][3] = v4.z;
    }

    Matrix3x4(const float* data)
    {
        this->data[0][0] = data[0];
        this->data[1][0] = data[1];
        this->data[2][0] = data[2];

        this->data[0][1] = data[3];
        this->data[1][1] = data[4];
        this->data[2][1] = data[5];

        this->data[0][2] = data[6];
        this->data[1][2] = data[7];
        this->data[2][2] = data[8];

        this->data[0][3] = data[9];
        this->data[1][3] = data[10];
        this->data[2][3] = data[11];
    }

    Matrix3x4(std::string_view str)
    {
        if(FromString(str))
            Matrix3x4(); // use default constructor if failed
    }

    /// column-major order
    Matrix3x4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23)
    {
        this->data[0][0] = m00;
        this->data[1][0] = m10;
        this->data[2][0] = m20;

        this->data[0][1] = m01;
        this->data[1][1] = m11;
        this->data[2][1] = m21;

        this->data[0][2] = m02;
        this->data[1][2] = m12;
        this->data[2][2] = m22;

        this->data[0][3] = m03;
        this->data[1][3] = m13;
        this->data[2][3] = m23;
    }

    Matrix3x4& operator = (const Matrix3x4& rhs)
    {
        this->data[0][0] = rhs.data[0][0];
        this->data[1][0] = rhs.data[1][0];
        this->data[2][0] = rhs.data[2][0];

        this->data[0][1] = rhs.data[0][1];
        this->data[1][1] = rhs.data[1][1];
        this->data[2][1] = rhs.data[2][1];

        this->data[0][2] = rhs.data[0][2];
        this->data[1][2] = rhs.data[1][2];
        this->data[2][2] = rhs.data[2][2];

        this->data[0][3] = rhs.data[0][3];
        this->data[1][3] = rhs.data[1][3];
        this->data[2][3] = rhs.data[2][3];

        return *this;
    }

    Matrix3x4& operator = (const Matrix3& rhs)
    {
        this->data[0][0] = rhs.data[0][0];
        this->data[1][0] = rhs.data[1][0];
        this->data[2][0] = rhs.data[2][0];

        this->data[0][1] = rhs.data[0][1];
        this->data[1][1] = rhs.data[1][1];
        this->data[2][1] = rhs.data[2][1];

        this->data[0][2] = rhs.data[0][2];
        this->data[1][2] = rhs.data[1][2];
        this->data[2][2] = rhs.data[2][2];

        this->data[0][3] = 0.0f;
        this->data[1][3] = 0.0f;
        this->data[2][3] = 0.0f;

        return *this;
    }

    Matrix3x4& operator = (const Matrix4& rhs)
    {
        this->data[0][0] = rhs.data[0][0];
        this->data[1][0] = rhs.data[1][0];
        this->data[2][0] = rhs.data[2][0];

        this->data[0][1] = rhs.data[0][1];
        this->data[1][1] = rhs.data[1][1];
        this->data[2][1] = rhs.data[2][1];

        this->data[0][2] = rhs.data[0][2];
        this->data[1][2] = rhs.data[1][2];
        this->data[2][2] = rhs.data[2][2];

        this->data[0][3] = rhs.data[0][3];
        this->data[1][3] = rhs.data[1][3];
        this->data[2][3] = rhs.data[2][3];

        return *this;
    }

    bool operator == (const Matrix3x4& rhs) const
    {
        const float* lhsData = &data[0][0];
        const float* rhsData = &rhs.data[0][0];

        for (int i = 0; i < 12; i++)
        {
            if (lhsData[i] != rhsData[i])
                return false;
        }

        return true;
    }

    Vector3 operator * (const Vector3& rhs) const
    {
        return Vector3(
            data[0][0] * rhs.x + data[0][1] * rhs.y + data[0][2] * rhs.z + data[0][3],
            data[1][0] * rhs.x + data[1][1] * rhs.y + data[1][2] * rhs.z + data[1][3],
            data[2][0] * rhs.x + data[2][1] * rhs.y + data[2][2] * rhs.z + data[2][3]
        );
    }

    Vector3 operator * (const Vector4& rhs) const
    {
        return Vector3(
            data[0][0] * rhs.x + data[0][1] * rhs.y + data[0][2] * rhs.z + data[0][3] * rhs.w,
            data[1][0] * rhs.x + data[1][1] * rhs.y + data[1][2] * rhs.z + data[1][3] * rhs.w,
            data[2][0] * rhs.x + data[2][1] * rhs.y + data[2][2] * rhs.z + data[2][3] * rhs.w
        );
    }

    Matrix3x4 operator + (const Matrix3x4& rhs) const
    {
        return Matrix3x4(
            Vector3(data[0][0] + rhs.data[0][0], data[1][0] + rhs.data[1][0], data[2][0] + rhs.data[2][0]),
            Vector3(data[0][1] + rhs.data[0][1], data[1][1] + rhs.data[1][1], data[2][1] + rhs.data[2][1]),
            Vector3(data[0][2] + rhs.data[0][2], data[1][2] + rhs.data[1][2], data[2][2] + rhs.data[2][2]),
            Vector3(data[0][3] + rhs.data[0][3], data[1][3] + rhs.data[1][3], data[2][3] + rhs.data[2][3])
        );
    }

    Matrix3x4 operator - (const Matrix3x4& rhs) const
    {
        return Matrix3x4(
            Vector3(data[0][0] - rhs.data[0][0], data[1][0] - rhs.data[1][0], data[2][0] - rhs.data[2][0]),
            Vector3(data[0][1] - rhs.data[0][1], data[1][1] - rhs.data[1][1], data[2][1] - rhs.data[2][1]),
            Vector3(data[0][2] - rhs.data[0][2], data[1][2] - rhs.data[1][2], data[2][2] - rhs.data[2][2]),
            Vector3(data[0][3] - rhs.data[0][3], data[1][3] - rhs.data[1][3], data[2][3] - rhs.data[2][3])
        );
    }

    Matrix3x4 operator * (float rhs) const 
    {
        return Matrix3x4(
            Vector3(data[0][0] * rhs, data[1][0] * rhs, data[2][0] * rhs),
            Vector3(data[0][1] * rhs, data[1][1] * rhs, data[2][1] * rhs),
            Vector3(data[0][2] * rhs, data[1][2] * rhs, data[2][2] * rhs),
            Vector3(data[0][3] * rhs, data[1][3] * rhs, data[2][3] * rhs)
        );
    }

    // TODO: Implement
    // Matrix3x4 operator * (const Matrix3x4& rhs) const;
    // Matrix4 operator * (const Matrix4& rhs) const;
            
    bool operator != (const Matrix3x4& rhs) const { return !(*this == rhs); }

    const float* Data() const { return &data[0][0]; }

    std::string ToString() const; 
    
    static const Matrix3x4 ZERO;
    static const Matrix3x4 IDENTITY;
private:
    bool FromString(std::string_view str);
};

} // namespace Pt