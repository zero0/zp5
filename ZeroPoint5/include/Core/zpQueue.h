#pragma once
#ifndef ZP_QUEUE_H
#define ZP_QUEUE_H

template<typename T, typename Allocator = zpDefaultGlobalAllocator>
class zpQueue
{
public:
    static const zp_size_t npos = -1;

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    typedef T* iterator;
    typedef const T* const_iterator;

    typedef Allocator allocator_value;
    typedef const allocator_value& allocator_const_reference;

    zpQueue();
    explicit zpQueue( allocator_const_reference allocator );
    explicit zpQueue( zp_size_t capacity );
    zpQueue( zp_size_t capacity, allocator_const_reference allocator );
    ~zpQueue();

    zp_size_t size() const;
    zp_bool isEmpty() const;

    void enqueue( const_reference val );
    value_type dequeue();
    
    zp_bool tryDequeue( reference val );

    const_reference peek() const;

    void clear();
    void reset();
    void reserve( zp_size_t size );
    void destroy();

private:
    void ensureCapacity( zp_size_t capacity );

    pointer m_data;
    zp_size_t m_size;
    zp_size_t m_head;
    zp_size_t m_tail;
    zp_size_t m_capacity;

    allocator_value m_allocator;
};

#include "zpQueue.inl"

#endif // !ZP_QUEUE_H
