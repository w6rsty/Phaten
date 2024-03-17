#include "IntVector.hpp"

#include <string>

namespace Pt {

const IntV2 IntV2::Zero = IntV2{0, 0};
const IntV2 IntV2::One = IntV2{1, 1};
const IntV2 IntV2::X = IntV2{1, 0};
const IntV2 IntV2::Y = IntV2{0, 1};

bool IntV2::FromString(std::string_view str)
{
    if (str.size() < 3) return false; // "x y" is the smallest possible string

    size_t space = str.find(' ');
    if (space == std::string::npos) return false;

    x = std::stoi(std::string{str.substr(0, space)});
    y = std::stoi(std::string{str.substr(space + 1)});
    return true;
}

std::string IntV2::ToString() const
{
    return std::to_string(x) + " " + std::to_string(y);
}

const IntV3 IntV3::Zero = IntV3{0, 0, 0};
const IntV3 IntV3::One = IntV3{1, 1, 1};
const IntV3 IntV3::X = IntV3{1, 0, 0};
const IntV3 IntV3::Y = IntV3{0, 1, 0};
const IntV3 IntV3::Z = IntV3{0, 0, 1};

bool IntV3::FromString(std::string_view str)
{
    if (str.size() < 5) return false; // "x y z" is the smallest possible string

    size_t space1 = str.find(' ');
    if (space1 == std::string::npos) return false;

    size_t space2 = str.find(' ', space1 + 1);
    if (space2 == std::string::npos) return false; 

    x = std::stoi(std::string{str.substr(0, space1)});
    y = std::stoi(std::string{str.substr(space1 + 1, space2 - space1 - 1)});
    z = std::stoi(std::string{str.substr(space2 + 1)});
    return true;
}

std::string IntV3::ToString() const
{
    return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z);
}

} // namespace Pt