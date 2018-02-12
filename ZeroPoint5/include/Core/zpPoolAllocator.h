#pragma once
#ifndef ZP_POOL_ALLOCATOR_H
#define ZP_POOL_ALLOCATOR_H

class zpPoolAllocator
{
    public:
        zpPoolAllocator();
        ~zpPoolAllocator();

        void* allocate( zp_size_t size );
        void free( void* ptr );

        zp_size_t getMemoryUsed() const;
        zp_size_t getTotalMemory() const;
        zp_size_t getNumAllocations() const;

    private:
        void* m_tlsf;

        zp_size_t m_totalMemory;
        zp_size_t m_totalMemoryUsed;
        zp_size_t m_numAllocs;
};

#endif