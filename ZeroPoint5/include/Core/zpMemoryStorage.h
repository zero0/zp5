#pragma once
#ifndef ZP_MEMORY_STORAGE_H
#define ZP_MEMORY_STORAGE_H

template<zp_size_t HeapSize>
class zpHeapMemoryStorage
{
public:
    zpHeapMemoryStorage()
        : m_memory( zp_malloc( HeapSize ) )
    {
    }

    ~zpHeapMemoryStorage()
    {
        zp_free( m_memory );
        m_memory = ZP_NULL;
    }

    void* getMemory()
    {
        return m_memory;
    }

    zp_size_t getTotalMemory() const
    {
        return HeapSize;
    }

private:
    void* m_memory;
};

//
//
//

template<zp_size_t StackSize>
class zpStackMemoryStorage
{
public:
    void* getMemory()
    {
        return m_memory;
    }

    zp_size_t getTotalMemory() const
    {
        return StackSize;
    }

private:
    zp_byte m_memory[ StackSize ];
};

//
//
//

class zpDirectMemoryStorage
{
public:
    zpDirectMemoryStorage( void* memory, zp_size_t size );
    ~zpDirectMemoryStorage();

    void* getMemory();

    zp_size_t getTotalMemory() const;

private:
    zpDirectMemoryStorage();

    void* m_memory;
    zp_size_t m_size;
};


#endif // !ZP_MEMORY_STORAGE_H
