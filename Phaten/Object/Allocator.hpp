#pragma once

#include <cstddef>
#include <new>

namespace Pt {

struct AllocatorBlock;
struct AllocatorNode;

#define DEFAULT_ALLOCATE_INITIAL_CAPACITY 16

/// Header info, chained together
struct AllocatorBlock
{
    /// Size of allocated object
    size_t nodeSize;
    /// Numbers of Nodes in this block
    size_t capacity;
    /// Ptr to first free node
    AllocatorNode* free;
    // Next allocator block
    AllocatorBlock* next;
};

/// Just a wrapper of a pointer
struct AllocatorNode
{
    AllocatorNode* next;
};

/// Initialize a fixed-size allocator
AllocatorBlock* AllocatorInitialize(size_t nodeSize, size_t initialCapacity = DEFAULT_ALLOCATE_INITIAL_CAPACITY);
/// Free all blocks in the chain
void AllocatorUninitialize(AllocatorBlock* allocator);
/// Allocate a node, will construct new blocks if it reaches the capacity
void* AllocatorGet(AllocatorBlock* allocator);
/// Free one node
void AllocatorFree(AllocatorBlock* allocator, void* ptr);

template <typename T>
class Allocator
{
public:
    Allocator(size_t capacity = DEFAULT_ALLOCATE_INITIAL_CAPACITY) :
        allocator(nullptr)
    {
        if (capacity)
            Reserve(capacity);
    }

    ~Allocator()
    {
        Reset();
    }

    /// Reserve initial, only possible before allocating the first object
    void Reserve(size_t capacity)
    {
        if (!allocator)
            allocator = AllocatorInitialize(sizeof(T), capacity);
    }

    T* Allocate()
    {
        if (!allocator)
            AllocatorInitialize(sizeof(T));
        T* newObject = static_cast<T*>(AllocatorGet(allocator));
        new(newObject) T();

        return newObject;
    }

    T* Allocate(const T& object)
    {
        if (!allocator)
            AllocatorInitialize(sizeof(T));
        T* newObject = static_cast<T*>(AllocatorGet(allocator));
        new(newObject) T(object);

        return newObject;
    }

    void Free(T* object)
    {
        (object)->~T();
        AllocatorFree(allocator, object);
    }
    void Reset()
    {
        AllocatorUninitialize(allocator);
        allocator = nullptr;
    }
private:
    Allocator(const Allocator<T>& rhs);
    Allocator<T>& operator = (const Allocator<T>& rhs);

    AllocatorBlock* allocator;
};

}