
template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap()
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator_value() )
{
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( zp_size_t capacity )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator_value() )
{
    ensureCapacity( capacity, false );
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( zp_size_t capacity, comparer_const_reference cmp )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator_value() )
{
    ensureCapacity( capacity, false );
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( zp_size_t capacity, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 ),
    m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator )
{
    ensureCapacity( capacity, false );
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( zp_size_t capacity, comparer_const_reference cmp, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 ),
    m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator )
{
    ensureCapacity( capacity, false );
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( comparer_const_reference cmp )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator_value() )
{
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( comparer_const_reference cmp, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator )
{
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::zpMap( allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator )
{
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zpMap< Key, Value, Comparer, Allocator>::~zpMap()
{
    destroy();
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_size_t zpMap< Key, Value, Comparer, Allocator>::size() const
{
    return m_count - m_freeCount;
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_bool zpMap< Key, Value, Comparer, Allocator>::isEmpty() const
{
    return ( m_count - m_freeCount ) == 0;
}


template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_bool zpMap< Key, Value, Comparer, Allocator>::get( key_const_reference key, value_pointer_reference value ) const
{
    zp_size_t index = findIndex( key );
    const zp_bool found = index != npos;
    if( found )
    {
        value = &m_entries[ index ].value;
    }

    return found;
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
void zpMap< Key, Value, Comparer, Allocator>::set( key_const_reference key, value_const_reference value )
{
    if( m_buckets == ZP_NULL )
    {
        ensureCapacity( 4, false );
    }

    const zp_hash_t hash = m_cmp.getHash( key );
    zp_size_t index = hash % m_capacity;
    zp_size_t numSteps = 0;

    for( zp_size_t b = m_buckets[ index ]; b != npos; b = m_entries[ b ].next )
    {
        if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].key, key ) )
        {
            m_entries[ b ].value = zp_move( zp_forward( value ) );
            return;
        }
        ++numSteps;
    }

    zp_size_t newIndex;
    if( m_freeCount > 0 )
    {
        newIndex = m_freeList;
        m_freeList = m_entries[ newIndex ].next;
        --m_freeCount;
    }
    else
    {
        if( m_count == m_capacity )
        {
            ensureCapacity( m_capacity * 2, false );
            index = hash % m_capacity;
        }
        newIndex = m_count;
        ++m_count;
    }

    m_entries[ newIndex ].hash = hash;
    m_entries[ newIndex ].next = m_buckets[ index ];
    m_entries[ newIndex ].key = zp_move( zp_forward( key ) );
    m_entries[ newIndex ].value = zp_move( zp_forward( value ) );

    m_buckets[ index ] = newIndex;

    if( numSteps >= m_capacity )
    {
        ensureCapacity( m_capacity + 1, true );
    }
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_bool zpMap< Key, Value, Comparer, Allocator>::containsKey( key_const_reference key ) const
{
    const zp_size_t index = findIndex( key );
    return index != npos;
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_bool zpMap< Key, Value, Comparer, Allocator>::remove( key_const_reference key )
{
    zp_bool removed = false;

    if( m_buckets != ZP_NULL )
    {
        const zp_hash_t hash = m_cmp.getHash( key );
        const zp_size_t index = hash % m_capacity;
        zp_size_t removeIndex = npos;

        for( zp_size_t b = m_buckets[ index ]; b != npos; b = m_entries[ b ].next )
        {
            if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].key, key ) )
            {
                if( removeIndex == npos )
                {
                    m_buckets[ index ] = m_entries[ b ].next;
                }
                else
                {
                    m_entries[ removeIndex ].next = m_entries[ b ].next;
                }

                m_entries[ b ].hash = npos;
                m_entries[ b ].next = m_freeList;
                ( &m_entries[ b ].key )->~key_value();
                ( &m_entries[ b ].value )->~value_value();

                m_freeList = b;
                ++m_freeCount;

                removed = true;
                break;
            }

            removeIndex = b;
        }
    }

    return removed;
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
void zpMap< Key, Value, Comparer, Allocator>::reserve( zp_size_t size )
{
    ensureCapacity( size, false );
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
void zpMap< Key, Value, Comparer, Allocator>::clear()
{
    if( m_count > 0 )
    {
        for( zp_size_t i = 0; i < m_capacity; ++i )
        {
            m_buckets[ i ] = npos;
        }

        for( zp_size_t i = 0; i < m_count; ++i )
        {
            ( m_entries + i )->~zpMapEntry();
        }

        m_freeList = npos;
        m_freeCount = 0;
        m_count = 0;
    }
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
void zpMap< Key, Value, Comparer, Allocator>::destroy()
{
    clear();

    m_allocator.free( m_buckets );
    m_buckets = ZP_NULL;

    m_allocator.free( m_entries );
    m_entries = ZP_NULL;

    m_capacity = 0;
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
void zpMap< Key, Value, Comparer, Allocator>::ensureCapacity( zp_size_t capacity, zp_bool forceRehash )
{
    if( capacity > m_capacity )
    {
        zp_size_t* buckets = static_cast<zp_size_t*>( m_allocator.allocate( sizeof( zp_size_t ) * capacity ) );
        zpMapEntry* entries = static_cast<zpMapEntry*>( m_allocator.allocate( sizeof( zpMapEntry ) * capacity ) );
        zp_zero_memory_array( entries, capacity );

        for( zp_size_t i = 0; i < capacity; ++i )
        {
            buckets[ i ] = npos;
        }

        if( m_entries != ZP_NULL )
        {
            for( zp_size_t i = 0; i < m_count; ++i )
            {
                entries[ i ] = zp_move( zp_forward( m_entries[ i ] ) );
            }

            if( forceRehash )
            {
                for( zp_size_t i = 0; i < m_count; ++i )
                {
                    if( entries[ i ].hash != npos )
                    {
                        entries[ i ].hash = m_cmp.getHash( entries[ i ].key );
                    }
                }
            }

            for( zp_size_t i = 0; i < m_count; ++i )
            {
                if( entries[ i ].hash != npos )
                {
                    zp_size_t index = entries[ i ].hash % capacity;
                    entries[ i ].next = buckets[ index ];
                    buckets[ index ] = i;
                }
            }

            m_allocator.free( m_entries );
            m_entries = ZP_NULL;
        }

        if( m_buckets != ZP_NULL )
        {
            m_allocator.free( m_buckets );
            m_buckets = ZP_NULL;
        }

        m_entries = entries;
        m_buckets = buckets;

        m_capacity = capacity;
    }
}

template<typename Key, typename Value, typename Comparer, typename Allocator>
zp_size_t zpMap< Key, Value, Comparer, Allocator>::findIndex( key_const_reference key ) const
{
    zp_size_t index = npos;
 
    if( m_buckets != ZP_NULL )
    {
        const zp_hash_t hash = m_cmp.getHash( key );
        for( zp_size_t b = m_buckets[ hash % m_capacity ]; b != npos; b = m_entries[ b ].next )
        {
            if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].key, key ) )
            {
                index = b;
                break;
            }
        }
    }

    return index;
}