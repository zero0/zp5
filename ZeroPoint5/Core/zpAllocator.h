#pragma once
#ifndef ZP_ALLOCATOR_H
#define ZP_ALLOCATOR_H

ZP_PURE_INTERFACE zpAllocator
{
public:
    virtual void* allocate( zp_size_t size ) = 0;
    virtual void free( void* ptr ) = 0;

    virtual zp_size_t getMemoryUsed() const = 0;
    virtual zp_size_t getTotalMemory() const = 0;
};

#endif // !ZP_ALLOCATOR_H
