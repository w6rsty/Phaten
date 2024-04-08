#pragma once

#include <map>
#include <string>
#include <type_traits>

#include "Object/Allocator.hpp"
#include "Ptr.hpp"
#include "IO/StringHash.hpp"

namespace Pt {

class Object;
class ObjectFactory;
template <typename T, typename Enable = std::enable_if_t<std::is_base_of_v<Object, T>>>
class ObjectFactoryImpl;

class Object : RefCounted
{
public: 
    virtual ~Object() = default;

    virtual void ReleaseRef() override;

    /// Get hash of the type name.
    virtual StringHash Type() const = 0;
    /// Get type name.
    virtual std::string_view TypeName() const = 0;

    /// Register an object which can accessed globally.
    static void RegisterSubsystem(Object* subsystem);
    /// Register subsystem by object type.
    template <typename T> static T* Subsystem()
    { 
        return static_cast<T*>(Subsystem(T::TypeStatic()));
    }
    /// Remove subsystem by pointer.
    static void RemoveSubsystem(Object* subsystem);
    /// Remove subsystem by type.
    static void RemoveSubsystem(StringHash type);

    /// Register an object factory by type.(Allocator capacity can be set). 
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
    static void RegisterFactory(size_t capacity = DEFAULT_ALLOCATE_INITIAL_CAPACITY)
    { 
        RegisterFactory(new ObjectFactoryImpl<T>(capacity)); 
    }
    /// Create an object using factory.
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
    static T* FactoryCreate()
    {
        return static_cast<T*>(FactoryCreate(T::TypeStatic()));
    }

    /// Destroy an object through a factory if exists. If not, just delete.
    static void Destory(Object* object);
    /// Get name from type.Must be registered in factories.
    static std::string_view TypeNameFromType(StringHash type);
private:
    /// Register substem by name.
    static Object* Subsystem(StringHash type);
    /// Register an object factory.(Stored as ScopedPtr)
    static void RegisterFactory(ObjectFactory* factory);
    /// Create an object using factory.
    static Object* FactoryCreate(StringHash type);

    /// Registerd subsystem objects.
    static std::map<StringHash, Object*> sSubSystems;
    /// Registerd ObjectFactories.
    static std::map<StringHash, ScopedPtr<ObjectFactory>> sFactories;
};

/// Base class for object factory.
class ObjectFactory
{
public:
    virtual ~ObjectFactory() = default;

    virtual Object* Create() = 0;
    virtual void Destroy(Object* object) = 0;

    StringHash Type() const { return m_Type; }
    std::string_view TypeName() const { return m_Name; }
protected:
    /// Object type.
    StringHash m_Type;
    /// Object name.
    std::string m_Name;
};

/// Template implementaion for object factory.
template <typename T, typename Enable>
class ObjectFactoryImpl : public ObjectFactory
{
public:
    ObjectFactoryImpl(size_t capacity = DEFAULT_ALLOCATE_INITIAL_CAPACITY) :
        m_Allocator(capacity)
    {
        m_Type = T::TypeStatic();
        m_Name = T::TypeNameStatic();
    }

    virtual Object* Create() override { return m_Allocator.Allocate(); }
    virtual void Destroy(Object* object) override { m_Allocator.Free(static_cast<T*>(object)); }
private:
    Allocator<T> m_Allocator;
};

/// Add Type and TypeName for Object subclass.
#define OBJECT(typeName) \
private: \
    static const StringHash sType; \
    static const std::string sTypeName; \
public: \
    virtual StringHash Type() const override { return TypeStatic(); } \
    virtual std::string_view TypeName() const override { return TypeNameStatic(); } \
    static StringHash TypeStatic() { static const StringHash type(#typeName); return type; } \
    static std::string_view TypeNameStatic() { static std::string_view type(#typeName); return type; } \

} // namespace Pt