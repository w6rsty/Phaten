#include <iostream>
#include <cassert>
#include <string>

#include "Math/Vector.hpp"

int main()
{
    Pt::Vector3 v1{"1 2 3"};
    Pt::Vector3 v2{"4 5 6"};

    Pt::Vector3 v3 = v1.Cross(v2);

    std::cout << v3.ToString() << std::endl;
    return 0;
}