#pragma once

#include <type_traits>
#include <cassert>

namespace Pt {

/*
For intrusive reference counting, the object must be based on RefCounted.
The RefCounted class holds a structure (RefCount) to record the reference 
count numbers.

Basic rules:
A SharedPtr will contribute to a strong reference count.
A WeakPtr will contribute to a weak reference count.

When the STRONG REFERENCE count reduces to zero, a RefCounted-based class
deconstructs and frees the reference counting structure. For SharedPtr, 
when it leaves a scope, it will release the strong reference, so no 
reference exists when the pointed objects exist.

WeakPtr does not contribute to the strong reference, so it does not control
the  deconstruction of the pointed object. When a RefCounted-based class
deconstructs, it will check if any WeakPtr still holds a reference to itself
by checking the weakRefs count. In this case, it will only deconstruct the 
object if weakRefs != 0, but KEEP the reference counting structure and mark
its expired flag as true. So a WeakPtr may still exist while its pointed
object is gone. Therefore, WeakPtr holds a refCount pointer to keep track of
the object status. When the last WeakPtr is gone, its destructor will be
responsible for the deallocation of the reference counting structure.
*/

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
    /// Only provide function to release strong reference.
    /// WeakPtr does the weak reference release job 
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
/// Destroyed after leave its scope and release the strong reference
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
    ret.StaticCast(rhs); // This pointer should have StaticCast() function.
    return ret;    
}

/// Perform a dynamic cast between shared pointers of two types.
template <typename T, typename U>
SharedPtr<T> DynamicCast(const SharedPtr<U>& rhs)
{
    SharedPtr<T> ret;
    ret.DynamicCast(rhs); // This pointer should have DynamicCast() function.
    return ret;    
}

/// Pointer which holds a weak reference to a RefCounted subclass.
/// Compiled only when T is base of RefCounted.
template <typename T,
          typename = std::enable_if_t<std::is_base_of_v<RefCounted, T>>>
class WeakPtr
{
public:
    WeakPtr() : 
        ptr(nullptr),
        refCount(nullptr)
    {
    }

    /// Construct from a raw pointer
    WeakPtr(T* ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_; // See operator= overload below.
    }

    /// Construct from another WeakPtr
    WeakPtr(const WeakPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_; // See operator= overload below.
    }

    /// Construct from a SharedPtr
    WeakPtr(const SharedPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr) // See operator= overload below.
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
        if (Get() == rhs) // Self check.
            return *this;

        Reset();
        ptr = rhs; // Just copy the object poiner.
        refCount = ptr ? ptr->RefCountPtr() : nullptr; // copy RefCount if not nullptr
        if (refCount)
            ++(refCount->weakRefs); // Add weak reference if not nullptr
        return *this;
    }

    /// Assign from another WeakPtr.
    WeakPtr<T>& operator = (const WeakPtr<T>& rhs)
    {
        if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.ptr; // Just copy pointer as above
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
        return *this;
    }

    /// Assign from a SharedPtr.
    WeakPtr<T>& operator = (const SharedPtr<T>& rhs)
    {   if (*this == rhs)
            return *this;

        Reset();
        ptr = rhs.Get(); // Just get and copy pointer as above.
        refCount = ptr ? ptr->RefCountPtr() : nullptr;
        if (refCount)
            ++(refCount->weakRefs);
        return *this;
    }

    /// Release a weak reference and reset object poiner.
    /// Notices that object may have been deconstructed.
    /// Free the RefCount structure if this is the last weak reference.
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
        *this = static_cast<T*>(rhs.ptr); // operator= overload
    }

    template <typename U>
    void DynamicCast(const WeakPtr<U>& rhs)
    {
        Reset(); // dynamic_cast below may failed and return nullptr.
        //  So reset first to preven previous object's memory leak.
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
        // Pointed Object may have been deconstructed.
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
    /// Return whether the pointed object has been destroyed.
    bool IsExpired() const { return refCount && refCount->expired; }
private:
    /// Object pointer.
    T* ptr;
    /// Reference counting structure.
    RefCount* refCount;
};

template <typename T, typename U>
WeakPtr<T> StaticCast(const WeakPtr<U>& rhs)
{
    WeakPtr<T> ret;
    ret.StaticCast(rhs);
    return ret;
}

template <typename T, typename U>
WeakPtr<T> DynamicCast(const WeakPtr<U>& rhs)
{
    WeakPtr<T> ret;
    ret.DynamicCast(rhs);
    return ret;
}

} // namespace Pt