#pragma once
#ifndef ZP_BLOCK_ALLOCATOR_H
#define ZP_BLOCK_ALLOCATOR_H

struct zpMemoryBlock
{
    zp_size_t size;
    zp_size_t alignedSize;
    zpMemoryBlock* next;
    zpMemoryBlock* prev;
};

class zpBlockAllocator : public zpAllocator
{
public:
    zpBlockAllocator( zp_size_t size );
    ~zpBlockAllocator();

    void* allocate( zp_size_t size );
    void free( void* ptr );

    zp_size_t getMemoryUsed() const;
    zp_size_t getTotalMemory() const;

private:
    zp_size_t m_totalMemory;
    zp_size_t m_totalAlignedMemory;

    zp_size_t m_allocatedMemorySize;
    zp_size_t m_numAllocs;
    zp_size_t m_memUsed;

    zp_byte* m_allMemory;
    zp_byte* m_alignedMemory;

    //zpMemoryBlock* m_blockTable[ ZP_MEMORY_BLOCK_TABLE_SIZE ];
    //zpMemoryBlock* m_freeTable[ ZP_MEMORY_BLOCK_TABLE_SIZE ];

    zpMemoryBlock* m_memoryBlockHead;
};

#endif // !ZP_BLOCK_ALLOCATOR_H
