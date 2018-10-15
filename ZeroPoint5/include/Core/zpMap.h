#pragma once
#ifndef ZP_MAP_H
#define ZP_MAP_H

template<typename Key, typename Value, typename Comparer = zpDefaultEqualityComparer<Key>, typename Allocator = zpDefaultGlobalAllocator >
class zpMap
{
public:
    static const zp_size_t npos = -1;

    typedef Key key_value;
    typedef Key& key_reference;
    typedef const Key& key_const_reference;

    typedef Value value_value;
    typedef Value& value_reference;
    typedef const Value& value_const_reference;

    typedef Value* value_pointer;
    typedef Value*& value_pointer_reference;
    typedef const Value* value_const_pointer;

    typedef Allocator allocator_value;
    typedef const Allocator& allocator_const_reference;

    typedef Comparer comparer_value;
    typedef const Comparer& comparer_const_reference;

    class zpMapIterator;
    class zpMapConstIterator;

    typedef zpMapIterator iterator;
    typedef zpMapConstIterator const_iterator;

    zpMap();
    explicit zpMap( zp_size_t capacity );
    zpMap( zp_size_t capacity, comparer_const_reference cmp );
    zpMap( zp_size_t capacity, allocator_const_reference allocator );
    zpMap( zp_size_t capacity, comparer_const_reference cmp, allocator_const_reference allocator );
    explicit zpMap( comparer_const_reference cmp );
    zpMap( comparer_const_reference cmp, allocator_const_reference allocator );
    explicit zpMap( allocator_const_reference allocator );
    ~zpMap();

    zp_size_t size() const;
    zp_bool isEmpty() const;

    zp_bool get( key_const_reference key, value_pointer_reference value ) const;
    void set( key_const_reference key, value_const_reference value );

    void setAll( const zpMap< Key, Value, Comparer, Allocator>& other );

    zp_bool containsKey( key_const_reference key ) const;

    zp_bool remove( key_const_reference key );

    void reserve( zp_size_t size );

    void clear();
    void destroy();

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

private:
    void ensureCapacity( zp_size_t capacity, zp_bool forceRehash );
    zp_size_t findIndex( key_const_reference key ) const;

    struct zpMapEntry
    {
        zp_hash_t hash;
        zp_size_t next;
        key_value key;
        value_value value;
    };

    zpMapEntry* m_entries;
    zp_size_t* m_buckets;

    zp_size_t m_capacity;
    zp_size_t m_count;
    zp_size_t m_freeCount;
    zp_size_t m_freeList;

    comparer_value m_cmp;
    allocator_value m_allocator;

    typedef zpMap< Key, Value, Comparer, Allocator> self;

public:
    class zpMapIterator
    {
    private:
        zpMapIterator();
        zpMapIterator( const self* map, zp_size_t index );

    public:
        ~zpMapIterator();

        void operator++();
        void operator++( zp_int );

        key_const_reference key() const;
        
        value_reference value();
        value_const_reference value() const;

        zp_bool operator==( const zpMapIterator& other ) const;
        zp_bool operator!=( const zpMapIterator& other ) const;

    private:
        const self * m_map;
        zp_size_t m_next;
        zp_size_t m_current;

        friend class zpMap;
    };


    class zpMapConstIterator
    {
    private:
        zpMapConstIterator();
        zpMapConstIterator( const self* map, zp_size_t index );

    public:
        ~zpMapConstIterator();

        void operator++();
        void operator++( zp_int );

        key_const_reference key() const;
        value_const_reference value() const;

        zp_bool operator==( const zpMapConstIterator& other ) const;
        zp_bool operator!=( const zpMapConstIterator& other ) const;
    
    private:
        const self * m_map;
        zp_size_t m_next;
        zp_size_t m_current;

        friend class zpMap;
    };
};

#include "zpMap.inl"

#endif // !ZP_MAP_H
