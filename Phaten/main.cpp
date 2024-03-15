#include <iostream>
#include "Core/Ptr.hpp"

using namespace Pt;

class Entity : public RefCounted {};

int main()
{
    SharedPtr<Entity> entity = SharedPtr<Entity>(new Entity);

    WeakPtr<Entity> weak = WeakPtr<Entity>(entity);

    return 0;
}