#pragma once

#include <cassert>
#include <utility>
#include <type_traits>


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
    T& operator * () const { assert(ptr); return *ptr; }
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

template <typename T, typename... Args>
SharedPtr<T> CreateShared(Args&&... args)
{
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
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

/// Hold a strong reference to an array, non-intrusive reference counting
template <typename T>
class SharedArrayPtr
{
public:
    SharedArrayPtr() :
        ptr(nullptr),
        refCount(nullptr)
    {
    }

    /// Construct from a raw array pointer.
    SharedArrayPtr(T* ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    /// Copy construct.
    SharedArrayPtr(const SharedArrayPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    ~SharedArrayPtr()
    {
        Reset();
    }

    /// Assign from a raw array pointer.
    SharedArrayPtr<T>& operator = (T* rhs)
    {
        if (Get() == rhs) // Self check.
            return *this;

        Reset();
        if (rhs) // Check nullptr.
        {
            ptr = rhs;
            refCount = RefCounted::AllocateRefCount(); // Allocate reference counting structure here.
            if (refCount)
                ++(refCount->refs);
        }

        return *this;
    }

    /// Assign from another SharedArrayPtr.
    SharedArrayPtr<T>& operator = (const SharedArrayPtr<T>& rhs)
    {
        if (*this == rhs) // Self check.
            return *this;

        Reset();
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->refs);
        
        return *this;
    }

    bool operator == (const SharedArrayPtr<T>& rhs) const { return ptr == rhs.ptr; }
    bool operator != (const SharedArrayPtr<T>& rhs) const { return !(*this == rhs); }
    T* operator -> () const { assert(ptr); return ptr; }
    T& operator * () const { assert(ptr); return ptr; }
    operator T* () const { return Get(); }

    void Reset()
    {
        if (refCount)
        {   
            --(refCount->refs);
            if (refCount->refs == 0) // Because T is not based on RefCounted, we need to handle the deconstruct manually.
            {
                refCount->expired = true; // Mark expired, may still have WeakPtr references.
                delete[] ptr;
                if (refCount->weakRefs == 0) // When no WeakPtr reference, free the reference counting structure.
                    RefCounted::FreeRefCount(refCount);
            }
        }

        ptr = nullptr;
        refCount = nullptr;
    }

    template <typename U>
    void StaticCast(const SharedArrayPtr<U>& rhs)
    {
        Reset();
        ptr = static_cast<T*>(rhs.Get());
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->refs);
    }

    template <typename U>
    void ReinterpretCast(const SharedArrayPtr<U>& rhs)
    {
        Reset();
        ptr = dynamic_cast<T*>(rhs.Get());
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->refs);
    }

    /// Return raw pointer.
    T* Get() const { return ptr; }
    /// Return numbers of strong reference counting.
    unsigned Refs() const { return refCount ? refCount->refs : 0; }
    /// Return numbers of weak reference counting.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return reference counting structure pointer.
    RefCount* RefCountPtr() const { return refCount; } 
    /// Return if ptr is null.
    bool IsNull() const { return ptr == nullptr; }
private:
    /// Prevent direct assignment form an array pointer of different type.
    template <typename U> SharedArrayPtr<T>& operator = (const SharedPtr<U>& rhs);
    /// Object pointer.
    T* ptr;
    /// reference counting structure.
    RefCount* refCount;
};

template <typename T, typename U>
SharedArrayPtr<T> StaticCast(const SharedArrayPtr<U>& rhs)
{
    SharedArrayPtr<T> ret;
    ret.StaticCast(rhs);
    return ret;
}

template <typename T, typename U>
SharedArrayPtr<T> ReinterpretCast(const SharedArrayPtr<U>& rhs)
{
    SharedArrayPtr<T> ret;
    ret.ReinterpretCast(rhs);
    return ret;
}

template <typename T>
class WeakArrayPtr
{
public:
    WeakArrayPtr() :
        ptr(nullptr),
        refCount(nullptr)
    {
    }

    WeakArrayPtr(const WeakArrayPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    WeakArrayPtr(const SharedArrayPtr<T>& ptr_) :
        ptr(nullptr),
        refCount(nullptr)
    {
        *this = ptr_;
    }

    ~WeakArrayPtr()
    {
        Reset();
    }

    WeakArrayPtr<T>& operator = (const WeakArrayPtr<T>& rhs)
    {
        if (ptr == rhs.ptr && refCount == rhs.refCount)
            return *this;

        Reset();
        ptr = rhs.ptr;
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);

        return *this;
    }

    WeakArrayPtr<T>& operator = (const SharedArrayPtr<T>& rhs)
    {
        if (ptr == rhs.Get() && refCount == rhs.RefCountPtr())
            return *this;

        Reset();
        ptr = rhs.Get();
        refCount = rhs.RefCountPtr();
        if (refCount)
            ++(refCount->weakRefs);

        return *this;
    }

    T* operator -> () const
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return rawPtr;
    }

    T& operator * () const
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return *rawPtr;
    }

    T& operator [] (size_t index)
    {
        T* rawPtr = Get();
        assert(rawPtr);
        return rawPtr[index];
    }

    const T& operator [] (size_t index) const
    {
        const T* rawPtr = Get();
        assert(rawPtr);
        return rawPtr[index];
    }

    bool operator == (const WeakArrayPtr<T>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    bool operator == (const SharedArrayPtr<T>& rhs) const { return ptr == rhs.ptr && refCount == rhs.refCount; }
    bool operator != (const WeakArrayPtr<T>& rhs) const { return !(*this == rhs); }
    bool operator != (const SharedArrayPtr<T>& rhs) const { return !(*this == rhs); }
    operator bool () const { return Get() != nullptr; }

    void Reset()
    {
        if (refCount)
        {
            --(refCount->weakRefs);
            // expired flag set to true means all strong reference have gone,
            // so last WeakPtr is responsible for deallocating the RefCount.
            if (refCount->expired && refCount->weakRefs == 0)
                RefCounted::FreeRefCount(refCount);
        }

        ptr = nullptr;
        refCount = nullptr;
    }

    template <typename U>
    void StaticCast(const WeakArrayPtr<U>& rhs)
    {
        Reset();
        ptr = static_cast<T*>(rhs.Get());
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
    }

    template <typename U>
    void ReinterpretCast(const WeakArrayPtr<U>& rhs)
    {
        Reset();
        ptr = reinterpret_cast<T*>(rhs);
        refCount = rhs.refCount;
        if (refCount)
            ++(refCount->weakRefs);
    }

    /// Return raw pointer.
    T* Get() const
    {
        if (!refCount || refCount->expired)
            return nullptr;
        else
            return ptr;
    }
    /// Return numbers of strong reference counting.
    unsigned Refs() const { return (refCount && refCount->refs >= 0) ? refCount->refs : 0; }
    /// Return numbers of weak reference counting.
    unsigned WeakRefs() const { return refCount ? refCount->weakRefs : 0; }
    /// Return if ptr is null.
    bool IsNull() const { return ptr == nullptr; }
    /// Return whether the array has been destroyed.
    bool IsExpired() const { return refCount ? refCount->expired : false; }
private:
    template <typename U> WeakArrayPtr(const WeakArrayPtr<U>& rhs);
    /// Pointer to array
    T* ptr;
    /// Pointer to reference counting structure
    RefCount* refCount;
};

template <typename T, typename U>
WeakArrayPtr<T> StaticCast(const WeakArrayPtr<U>& rhs)
{
    WeakArrayPtr<T> ret;
    ret.StaticCast(rhs);
    return ret;
}

template <typename T, typename U>
WeakArrayPtr<T> ReinterpretCast(const SharedArrayPtr<U> &rhs)
{
    WeakArrayPtr<T> ret;
    ret.ReinterpretCast(rhs);
    return ret;
}

/// A pointer which deletes the object when it leaves the scope.
template <typename T>
class ScopedPtr
{
public:
    ScopedPtr() :
        ptr(nullptr)
    {
    }

    /// Construct from a raw pointer
    ScopedPtr(T* ptr_) :
        ptr(ptr_)
    {
    }

    /// Copy construct, Transfer ownership.
    ScopedPtr(const ScopedPtr<T>& ptr_) :
        ptr(ptr_.ptr)
    {
        const_cast<ScopedPtr<T>&>(ptr_).ptr = nullptr;
    }

    ~ScopedPtr()
    {
        delete ptr;
    }

    ScopedPtr<T>& operator = (T* rhs)
    {
        delete ptr; 
        ptr = rhs;
        return *this;
    }

    ScopedPtr<T>& operator = (const ScopedPtr<T>& rhs)
    {
        delete ptr;
        ptr = rhs.ptr;
        const_cast<ScopedPtr<T>&>(rhs).ptr = nullptr;
        return *this;
    }

    /// Detach the object and return the raw pointer.
    T* Detach()
    {
        T* ret = ptr;
        ptr = nullptr;
        return ret;
    }

    /// Reset to null, delete the object if has.
    void Reset()
    {
        *this = nullptr;
    }

    T* operator -> () const { assert(ptr); return ptr; }
    T& operator *  () const { assert(ptr); return *this; }
    operator T* () const { return ptr; }

    T* Get() const { return ptr; }
    bool IsNull() const { return ptr == nullptr; } 
private:
    /// Pointer to the object.
    T* ptr;
};

template <typename T, typename... Args>
ScopedPtr<T> CreateScoped(Args&&... args)
{
    return ScopedPtr<T>(new T(std::forward<Args>(args)...));
}

} // namespace Pt