#pragma once
#ifndef ZP_HASH_SET_H
#define ZP_HASH_SET_H

template< typename T, typename Comparer = zpDefaultEqualityComparer<T>, typename Allocator = zpDefaultGlobalAllocator >
class zpHashSet
{
public:
    static const zp_size_t npos = -1;

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;

    class zpHashSetIterator;
    class zpHashSetConstIterator;

    typedef zpHashSetIterator iterator;
    typedef zpHashSetConstIterator const_iterator;

    typedef Comparer comparer_value;
    typedef const Comparer& comparer_const_reference;

    typedef Allocator allocator_value;
    typedef const allocator_value& allocator_const_reference;

    zpHashSet();
    explicit zpHashSet( zp_size_t capacity );
    explicit zpHashSet( comparer_const_reference cmp );
    explicit zpHashSet( allocator_const_reference allocator );
    zpHashSet( zp_size_t capacity, comparer_const_reference cmp );
    zpHashSet( zp_size_t capacity, allocator_const_reference allocator );
    zpHashSet( zp_size_t capacity, comparer_const_reference cmp, allocator_const_reference allocator );
    zpHashSet( comparer_const_reference cmp, allocator_const_reference allocator );
    ~zpHashSet();

    zp_bool add( const_reference value );
    zp_bool remove( const_reference value );

    void unionWith( const_pointer ptr, zp_size_t count );
    void unionWith( const_iterator begin, const_iterator end );

    void exceptWith( const_pointer ptr, zp_size_t count );
    void exceptWith( const_iterator begin, const_iterator end );

    zp_bool contains( const_reference value ) const;

    zp_size_t size() const;
    zp_bool isEmpty() const;

    void clear();
    void reserve( zp_size_t size );
    void destroy();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

private:
    void ensureCapacity( zp_size_t capacity, zp_bool forceRehash );
    zp_size_t findIndex( const_reference value ) const;

    struct zpHashSetEntry
    {
        zp_hash_t hash;
        zp_size_t next;
        value_type value;
    };

    zpHashSetEntry* m_entries;
    zp_size_t* m_buckets;

    zp_size_t m_capacity;
    zp_size_t m_count;
    zp_size_t m_freeCount;
    zp_size_t m_freeList;

    comparer_value m_cmp;
    allocator_value m_allocator;

    typedef zpHashSet< T, Comparer, Allocator > self;

public:
    class zpHashSetIterator
    {
    private:
        zpHashSetIterator();
        zpHashSetIterator( const self* hashSet, zp_size_t index );

    public:
        ~zpHashSetIterator();

        void operator++();
        void operator++( zp_int );

        pointer operator->();
        const_pointer operator->() const;

        const_reference operator*() const;

        zp_bool operator==( const zpHashSetIterator& other ) const;
        zp_bool operator!=( const zpHashSetIterator& other ) const;

    private:
        const self* m_hashSet;
        zp_size_t m_next;
        zp_size_t m_current;

        friend class zpHashSet;
    };

    class zpHashSetConstIterator
    {
    private:
        zpHashSetConstIterator();
        zpHashSetConstIterator( const self* hashSet, zp_size_t index );

    public:
        ~zpHashSetConstIterator();

        void operator++();
        void operator++( zp_int );

        const_pointer operator->() const;
        const_reference operator*() const;
        
        zp_bool operator==( const zpHashSetConstIterator& other ) const;
        zp_bool operator!=( const zpHashSetConstIterator& other ) const;

    private:
        const self* m_hashSet;
        zp_size_t m_next;
        zp_size_t m_current;

        friend class zpHashSet;
    };
};

#include "zpHashSet.inl"

#endif // !ZP_HASH_SET_H
