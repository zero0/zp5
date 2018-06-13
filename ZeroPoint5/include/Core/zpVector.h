#pragma once
#ifndef ZP_VECTOR_H
#define ZP_VECTOR_H

template< typename T, typename Allocator = zpDefaultGlobalTemplateArrayAllocator< T > >
class zpVector
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

    zpVector();
    explicit zpVector( allocator_const_reference );
    zpVector( zp_size_t capacity, allocator_const_reference = allocator_value() );
    ~zpVector();

    reference operator[]( zp_size_t index );
    const_reference operator[]( zp_size_t index ) const;

    const_reference at( zp_size_t index ) const;

    zp_size_t size() const;
    zp_bool isEmpty() const;
    zp_bool isFixed() const;

    void pushBack( const_reference val );
    reference pushBackEmpty();

    void pushFront( const_reference val );
    reference pushFrontEmpty();

    void popBack();
    void popFront();

    void eraseAt( zp_size_t index );
    void eraseAtSwapBack( zp_size_t index );
    zp_bool erase( const_reference val );
    zp_bool eraseSwapBack( const_reference val );
    zp_size_t eraseAll( const_reference val );

    void clear();
    void reset();
    void reserve( zp_size_t size );
    void destroy();

    zp_size_t indexOf( const_reference val ) const;
    zp_size_t lastIndexOf( const_reference val ) const;

    T& front();
    T& back();

    const_reference front() const;
    const_reference back() const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

private:
    void ensureCapacity( zp_size_t size );

    pointer m_data;
    zp_size_t m_size;
    zp_size_t m_capacity;

    Allocator m_allocator;
};

#include "zpVector.inl"

#endif // !ZP_VECTOR_H
