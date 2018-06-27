#pragma once
#ifndef ZP_STACK_H
#define ZP_STACK_H

template<typename T, typename Allocator = zpDefaultGlobalAllocator>
class zpStack
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

    zpStack();
    explicit zpStack( allocator_const_reference allocator );
    explicit zpStack( zp_size_t capacity );
    zpStack( zp_size_t capacity, allocator_const_reference allocator );
    ~zpStack();

    zp_size_t size() const;
    zp_bool isEmpty() const;


    void push( const_reference val );
    value_type pop();

    zp_bool tryPop( reference val );

    const_reference peek() const;

    void clear();
    void reset();
    void reserve( zp_size_t size );
    void destroy();

private:
    void ensureCapacity( zp_size_t capacity );

    pointer m_data;
    zp_size_t m_size;
    zp_size_t m_capacity;

    allocator_value m_allocator;
};

#include "zpStack.inl"

#endif // !ZP_STACK_H
