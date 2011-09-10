#ifndef MEMORY_POOL_
#define MEMORY_POOL_

#include <vector>

template <typename T, unsigned int capacity>
class MemoryPool
{
public:
    MemoryPool()
    {
        unusedIndex = capacity-1;
        for (unsigned int i = 0; i < capacity; i++)
        {
            unused[i] = &memory[i];
        }
    }
    T* alloc()
    {
        return unused[unusedIndex--];
    }
    void free(T* ptr)
    {
        unused[++unusedIndex] = ptr;
    }

private:
    MemoryPool& operator=(const MemoryPool& rhs);
    MemoryPool(const MemoryPool& rhs);

    int unusedIndex;
    T* unused[capacity];
    T memory[capacity];
};

#endif // MEMORY_POOL_
