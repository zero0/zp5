#pragma once
#ifndef ZP_MEMORY_ALLOCATOR_H
#define ZP_MEMORY_ALLOCATOR_H

ZP_PURE_INTERFACE zpIMemoryAllocator
{
public:
    virtual void* allocate( zp_size_t size ) = 0;
    virtual void free( const void* ptr ) = 0;

    virtual zp_size_t getMemoryUsed() const = 0;
    virtual zp_size_t getTotalMemory() const = 0;

    virtual zp_uint getNumAllocations() const = 0;
    virtual zp_uint getNumFrees() const = 0;
};

template<typename Storage, typename Policy>
class zpMemoryAllocator : public zpIMemoryAllocator
{
    ZP_NON_COPYABLE( zpMemoryAllocator );

public:
    zpMemoryAllocator();
    ~zpMemoryAllocator();

    void* allocate( zp_size_t size );
    void free( const void* ptr );

    void setup();
    void teardown();

    zp_size_t getMemoryUsed() const;
    zp_size_t getTotalMemory() const;

    zp_uint getNumAllocations() const;
    zp_uint getNumFrees() const;

private:
    Storage m_storage;
    Policy m_policy;

    zp_uint m_numAllocations;
    zp_uint m_numFrees;
};

//
//
//

template<typename Storage, typename Policy>
zpMemoryAllocator<Storage, Policy>::zpMemoryAllocator()
    : m_storage()
    , m_policy()
    , m_numAllocations( 0 )
    , m_numFrees( 0 )
{
}

template<typename Storage, typename Policy>
zpMemoryAllocator<Storage, Policy>::~zpMemoryAllocator()
{
}

template<typename Storage, typename Policy>
void* zpMemoryAllocator<Storage, Policy>::allocate( zp_size_t size )
{
    ++m_numAllocations;
    void* ptr = m_policy.allocate( size );
    return ptr;
}

template<typename Storage, typename Policy>
void zpMemoryAllocator<Storage, Policy>::free( const void* ptr )
{
    ++m_numFrees;
    m_policy.free( ptr );
}

template<typename Storage, typename Policy>
void zpMemoryAllocator<Storage, Policy>::setup()
{
    m_policy.setup( m_storage.getMemory(), m_storage.getTotalMemory() );
}

template<typename Storage, typename Policy>
void zpMemoryAllocator<Storage, Policy>::teardown()
{
    m_policy.teardown();
}

template<typename Storage, typename Policy>
zp_size_t zpMemoryAllocator<Storage, Policy>::getMemoryUsed() const
{
    return m_policy.getMemoryUsed();
}

template<typename Storage, typename Policy>
zp_size_t zpMemoryAllocator<Storage, Policy>::getTotalMemory() const
{
    return m_storage.getTotalMemory();
}

template<typename Storage, typename Policy>
zp_uint zpMemoryAllocator<Storage, Policy>::getNumAllocations() const
{
    return m_numAllocations;
}

template<typename Storage, typename Policy>
zp_uint zpMemoryAllocator<Storage, Policy>::getNumFrees() const
{
    return m_numFrees;
}

#endif // !ZP_MEMORY_ALLOCATOR_H
