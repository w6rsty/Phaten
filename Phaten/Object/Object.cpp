#include "Object.hpp"

namespace Pt {

std::map<StringHash, Object*> Object::sSubSystems;
std::map<StringHash, ScopedPtr<ObjectFactory>> Object::sFactories;

void Object::ReleaseRef()
{
    assert(refCount && refCount->refs > 0);
    --(refCount->refs);
    if (refCount->refs == 0)
    {
        Destory(this);
    }   
}

void Object::RegisterSubsystem(Object* subsystem)
{
    if (!subsystem)
    {
        return;
    }
    sSubSystems[subsystem->Type()] = subsystem;
}

Object* Object::Subsystem(StringHash type)
{
    auto it = sSubSystems.find(type);
    return it != sSubSystems.end() ? it->second : nullptr;
}

void Object::RemoveSubsystem(Object* subsystem)
{
    if (!subsystem)
    {
        return;
    }

    auto it = sSubSystems.find(subsystem->Type());
    if (it != sSubSystems.end() && it->second == subsystem)
    {
        sSubSystems.erase(it);
    }
}

void Object::RemoveSubsystem(StringHash type)
{
    sSubSystems.erase(type);
}

void Object::RegisterFactory(ObjectFactory* factory)
{
    if (!factory)
    {
        return;
    }
    sFactories[factory->Type()] = factory;
}

Object* Object::FactoryCreate(StringHash type)
{
    auto it = sFactories.find(type);
    return it != sFactories.end() ? sFactories[type]->Create() : nullptr;
}

void Object::Destory(Object* object)
{
    if (!object)
    {
        return;
    }

    auto it = sFactories.find(object->Type());
    if (it != sFactories.end())
    {
        it->second->Destroy(object);
    }
    else {
        delete object;
    }
}

std::string_view Object::TypeNameFromType(StringHash type)
{
    auto it = sFactories.find(type);
    return it != sFactories.end() ? it->second->TypeName() : "";
}

} // namespace Pt