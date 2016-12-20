#pragma once
#ifndef ZP_VECTOR_H
#define ZP_VECTOR_H

template< typename T >
struct zpVectorAllocator
{
    T* allocate( zp_size_t count );
    void free( T* ptr );
};

template< typename T, typename Allocator = zpVectorAllocator< T > >
class zpVector
{
public:
    static const zp_size_t npos = -1;

    zpVector();
    explicit zpVector( const Allocator& allocator );
    zpVector( zp_size_t capacity, const Allocator& allocator = Allocator() );
    ~zpVector();

    T& operator[]( zp_size_t index );
    const T& operator[]( zp_size_t index ) const;

    const T& at( zp_size_t index ) const;

    zp_size_t size() const;
    zp_bool isEmpty() const;
    zp_bool isFixed() const;

    void pushBack( const T& val );
    T& pushBackEmpty();

    void pushFront( const T& val );
    T& pushFrontEmpty();

    void popBack();
    void popFront();

    void erase( zp_size_t index );
    zp_size_t eraseAll( const T& val );

    void clear();
    void reset();
    void reserve( zp_size_t size );
    void destroy();

    zp_size_t indexOf( const T& val ) const;
    zp_size_t lastIndexOf( const T& val ) const;

    T& front();
    T& back();

    const T& front() const;
    const T& back() const;

    T* begin();
    T* end();

    const T* begin() const;
    const T* end() const;

private:
    void ensureCapacity( zp_size_t size );

    T* m_data;
    zp_size_t m_size;
    zp_size_t m_capacity;

    Allocator m_allocator;
};

#include "zpVector.inl"

#endif // !ZP_VECTOR_H
