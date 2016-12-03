#pragma once
#ifndef ZP_STACK_ALLOCATOR_H
#define ZP_STACK_ALLOCATOR_H

struct zpStackBlock
{
    zp_byte* data;
    zp_size_t size;
};

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
class zpStackAllocator : public zpAllocator
{
public:
    zpStackAllocator();
    ~zpStackAllocator();

    void* allocate( zp_size_t size );
    void free( void* ptr );

    zp_size_t getMemoryUsed() const;
    zp_size_t getTotalMemory() const;

private:
    zp_size_t m_memUsed;
    zp_size_t m_stackCount;

    zp_byte m_data[ Size ];
    zpStackBlock m_stackBlocks[ StackBlocks ];
};

#include "zpStackAllocator.inl"

#endif // !ZP_STACK_ALLOCATOR_H
