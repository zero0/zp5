#pragma once
#ifndef ZP_BLOCK_ALLOCATOR_H
#define ZP_BLOCK_ALLOCATOR_H

struct zpMemoryChunk
{
    zp_size_t size;
    zp_size_t alignedSize;
    zpMemoryChunk* next;
    zpMemoryChunk* prev;
};

struct zpMemoryBlock
{
    zp_size_t size;
    zp_size_t alignedSize;
    zpMemoryBlock* next;
    zpMemoryBlock* prev;
    zpMemoryChunk* chunk;
};

class zpBlockAllocator : public zpAllocator
{
public:
    zpBlockAllocator( zp_size_t minChunckSize );
    ~zpBlockAllocator();

    void* allocate( zp_size_t size );
    void free( void* ptr );

    zp_size_t getMemoryUsed() const;
    zp_size_t getTotalMemory() const;
    zp_size_t getNumAllocations() const;

private:
    zp_size_t m_minChunkSize;

    zp_size_t m_numAllocs;
    zp_size_t m_totalMemoryUsed;
    zp_size_t m_totalMemory;

    zpMemoryChunk m_memoryChunkHead;
};

#endif // !ZP_BLOCK_ALLOCATOR_H
