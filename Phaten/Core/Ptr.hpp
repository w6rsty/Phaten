#pragma once

#include <type_traits>
#include <cassert>

namespace Pt {

/// Structure for reference counting
struct RefCount
{
    RefCount() :
        refs(0),
        weakRefs(0),
        expired(false)
    {}

    /// Number of strong reference.
    unsigned refs;
    /// Number of weak reference.
    unsigned weakRefs;
    /// A flag. The object is no longer safe afetr this is set to true.
    bool expired;
};

/// Base class for reference counting.
class RefCounted
{
public:
    /// Construct. RefCount is not allocated now but when needed.
    RefCounted();
    /// Destruct. Free RefCount if no weak references, else marks it expired.
    ~RefCounted();

    /// Add a strong reference (Allocate RefCount first time if neccessary).
    void AddRef();
    /// Release a strong reference. Destory the object when last strong reference is gone.
    virtual void ReleaseRef();

    /// Get strong reference count.
    unsigned Refs() const { return refCount ? refCount->refs : 0; }
    /// Get weak reference count.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Get reference counting structure.
    RefCount* RefCountPtr();

    /// Allocate a RefCount structure.
    static RefCount* AllocateRefCount();
    /// Free a RefCount structure
    static void FreeRefCount(RefCount* refCount);
private:
    /// Prevent copy construction.
    RefCounted(const RefCounted& rhs);
    /// Prevent assignment
    RefCounted& operator = (const RefCounted& rhs);
    /// Reference counting structure, allocated on demand.
    RefCount* refCount;
};

/// Pointer holds a strong reference to a RefCounted subclass.
/// Compiled only when T is base of RefCounted.
template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<RefCounted, T>>>
class SharedPtr
{
public:
    /// Construct a nullptr(Do nothing).
    SharedPtr() :
        ptr(nullptr)
    {
    }

    /// Construct from raw ptr.
    SharedPtr(T* ptr_) :
        ptr(nullptr)
    {   
        *this = ptr_; // See operator= overload below.
    }

    /// Copy construct
    SharedPtr(const SharedPtr<T>& ptr_) :
        ptr(nullptr)
    {
        *this = ptr_; // See operator= overload below.
    }

    ~SharedPtr()
    {
        Reset();
    }

    /// Assigned from raw pointer
    SharedPtr<T>& operator = (T* rhs)
    {
        if (Get() == rhs) // Self check.
            return *this;

        Reset();
        ptr = rhs; // Replace object pointer.
        if (ptr) // Add a strong reference if object exists.
            // Because T is base of RefCounted, we need to reinterpret
            // the pointer to call the AddRef() function.
            reinterpret_cast<RefCounted*>(ptr)->AddRef(); 
        return *this;
    }

    /// Assigned from another shared pointer
    SharedPtr<T>& operator = (const SharedPtr<T>& rhs)
    {
        if (*this == rhs) // Self check.
            return *this;
        
        Reset();
        ptr = rhs.ptr; // Just copy object pointer.
        if (ptr)
            reinterpret_cast<RefCounted*>(ptr)->AddRef();
        return *this;
    }

    /// Release the object and set the pointer to null.
    /// The object may be destroyed if it was the last reference.
    /// In short, revoke the reference to current object if it exists.
    void Reset()
    {
        if (ptr)
        {
            reinterpret_cast<RefCounted*>(ptr)->ReleaseRef();
            ptr = nullptr;
        }
    }

    bool operator == (T* rhs) const { return ptr == rhs; }
    bool operator == (const SharedPtr<T>& rhs) const { return ptr == rhs.ptr; }
    bool operator != (T* rhs) const { return !(*this == rhs); }
    bool operator != (const SharedPtr<T>& rhs) const { return !(*this == rhs); }
    /// Point to the object
    T* operator -> () const { assert(ptr); return ptr; }
    /// Dereference
    T& operator * () const { assert(ptr); return ptr; }
    /// Convert to T pointer
    operator T* () const { return ptr; }

    /// Return object pointer
    T* Get() const { return ptr; }
    /// Return strong reference count
    unsigned Refs() const { return ptr ?  ptr->Refs() : 0; }
    /// Return weak reference count
    unsigned WeakRefs() const { return ptr ? ptr->WeakRefs() : 0; }
    /// Return if it is a nullptr
    bool IsNull() const { return ptr == nullptr; }
private:
    T* ptr;
};

/// Perform a static cast between shared pointers of two types.
template <typename T, typename U>
SharedPtr<T> StaticCast(const SharedPtr<U>& rhs)
{
    SharedPtr<T> ret;
    ret.StaticCast(rhs);
    return ret;    
}

/// Perform a dynamic cast between shared pointers of two types.
template <typename T, typename U>
SharedPtr<T> DynamicCast(const SharedPtr<U>& rhs)
{
    SharedPtr<T> ret;
    ret.DynamicCast(rhs);
    return ret;    
}

/// Pointer which holds a weak reference to a RefCounted subclass.
/// Compiled only when T is base of RefCounted.
template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<RefCount, T>>>
class WeakPtr
{
public:
    WeakPtr() : 
        ptr(nullptr),
        refCount(nullptr)
    {
    }

    WeakPtr(const WeakPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    WeakPtr(const SharedPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    WeakPtr(T* ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    ~WeakPtr()
    {
        Reset();
    }

    /// Assigned from raw pointer
    WeakPtr<T>& operator = (T* rhs)
    {
        if (Get() == rhs)
            return *this;

        Reset();
        ptr = rhs;
        refCount = ptr ? ptr->RefCountPtr() : nullptr;
        if (refCount)
            ++(refCount->weakRefs);
        return *this;
    }

    WeakPtr<T>& operator = (const WeakPtr<T>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
        return *this;
    }

    WeakPtr<T>& operator = (const SharedPtr<T>& rhs)
    {   if (Get() == rhs.ptr)
            return *this;

        Reset();
        ptr = rhs.Get();
        refCount = ptr ? ptr->RefCountPtr() : nullptr;
        if (refCount)
            ++(refCount->weakRefs);
        return *this;
    }

    void Reset()
    {
        if (refCount)
        {
            --(refCount->weakRefs);
            if (refCount->expired && refCount->weakRefs == 0)
                RefCounted::FreeRefCount(refCount);
            ptr = nullptr;
            refCount = nullptr;
        }
    }

    template <typename U>
    void StaticCast(const WeakPtr<U>& rhs)
    {
        *this = static_cast<T*>(rhs.ptr);
    }

    template <typename U>
    void DynamicCast(const WeakPtr<U>& rhs)
    {
        Reset();
        T* rhsObject = dynamic_cast<T*>(rhs);
        if (rhsObject)
            *this = rhsObject;
    }

    bool operator == (T* rhs) const { return ptr == rhs; }
    bool operator == (const WeakPtr<T>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    bool operator == (const SharedPtr<T>& rhs) const { return ptr == rhs.Get(); }
    bool operator != (T* rhs) const { return !(*this == rhs); }
    bool operator != (const WeakPtr<T>& rhs) const { return !(*this == rhs); }
    bool operator != (const SharedPtr<T>& rhs) const { return !(*this == rhs); }
    T* operator -> () const { T* ret = Get(); assert(ret); return ret; }
    T& operator * () const { T* ret = Get(); assert(ret); return ret; }
    operator T* () const { return Get(); }

    /// Return object pointer
    T* Get() const 
    {
        if (refCount && !refCount->expired)
            return ptr;
        else
            return nullptr;
    }

    /// Return strong reference count
    unsigned Refs() const { return ptr ?  ptr->Refs() : 0; }
    /// Return weak reference count
    unsigned WeakRefs() const { return ptr ? ptr->WeakRefs() : 0; }
    /// Return if it is a nullptr
    bool IsNull() const { return ptr == nullptr; } 
    bool IsExpired() const { return refCount && refCount->expired; }
private:
    T* ptr;
    RefCount* refCount;
};

} // namespace Pt