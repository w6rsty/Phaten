#include "Matrix.hpp"

#include "IO/Assert.hpp"
#include "IO/StringUtils.hpp"

namespace Pt {

const Matrix3 Matrix3::ZERO {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
const Matrix3 Matrix3::IDENTITY {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

Matrix3::Matrix3(const Matrix4& matrix)
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

bool Matrix3::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');
    if (count != 9)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    // column major
    for (int i = 0; i < 9; ++i)
    {
        this->data[i % 3][i / 3] = strtof(ptr, &ptr);
    }

    return true;
}

// to a linear array
std::string Matrix3::ToString() const
{
    return FormatString("%g %g %g %g %g %g %g %g %g",
        this->data[0][0], this->data[1][0], this->data[2][0],
        this->data[0][1], this->data[1][1], this->data[2][1],
        this->data[0][2], this->data[1][2], this->data[2][2]);
}

std::ostream& operator << (std::ostream& os, const Matrix3& matrix)
{
    for (int j = 0; j < 3; ++j)
    {
        for (int i = 0; i < 3; ++i)
        {
            os << matrix.data[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

const Matrix4 Matrix4::ZERO {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
const Matrix4 Matrix4::IDENTITY {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

bool Matrix4::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');
    if (count != 16)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    // column major
    for (int i = 0; i < 16; ++i)
    {
        this->data[i % 4][i / 4] = strtof(ptr, &ptr);
    }

    return true;
}

std::string Matrix4::ToString() const
{
    return FormatString("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g",
        this->data[0][0], this->data[1][0], this->data[2][0], this->data[3][0],
        this->data[0][1], this->data[1][1], this->data[2][1], this->data[3][1],
        this->data[0][2], this->data[1][2], this->data[2][2], this->data[3][2],
        this->data[0][3], this->data[1][3], this->data[2][3], this->data[3][3]);
}

std::ostream& operator << (std::ostream& os, const Matrix4& matrix)
{
    for (int j = 0; j < 4; ++j)
    {
        for (int i = 0; i < 4; ++i)
        {
            os << matrix.data[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

const Matrix3x4 Matrix3x4::ZERO {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
const Matrix3x4 Matrix3x4::IDENTITY {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};

bool Matrix3x4::FromString(std::string_view str)
{
    std::string trimed = Trimed(str);
    int count = CountElements(trimed, ' ');
    if (count != 12)
    {
        PT_ASSERT_MSG(false, "Invalid string format");
        return false;
    }

    char* ptr = trimed.data();
    // column major
    for (int i = 0; i < 12; ++i)
    {
        this->data[i % 3][i / 3] = strtof(ptr, &ptr);
    }

    return true;
}

std::string Matrix3x4::ToString() const
{
    return FormatString("%g %g %g %g %g %g %g %g %g %g %g %g",
        this->data[0][0], this->data[1][0], this->data[2][0],
        this->data[0][1], this->data[1][1], this->data[2][1],
        this->data[0][2], this->data[1][2], this->data[2][2],
        this->data[0][3], this->data[1][3], this->data[2][3]);
}

} // namespace Pt