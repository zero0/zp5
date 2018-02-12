#include "Core/zpCore.h"

// allocator wrapper
zpPoolAllocator::zpPoolAllocator()
{
    
}

zpPoolAllocator::~zpPoolAllocator()
{

}

void* zpPoolAllocator::allocate( zp_size_t size )
{
    m_totalMemoryUsed += size;
    void* ptr = zp_tlsf_malloc( m_tlsf, size );
    return ptr;
}

void zpPoolAllocator::free( void* ptr )
{
    m_totalMemoryUsed -= zp_tlsf_block_size( ptr );
    zp_tlsf_free( m_tlsf, ptr );
}

zp_size_t zpPoolAllocator::getMemoryUsed() const
{
    return m_totalMemoryUsed;
}

zp_size_t zpPoolAllocator::getTotalMemory() const
{
    return m_totalMemory;
}

zp_size_t zpPoolAllocator::getNumAllocations() const
{
    return m_numAllocs;
}
