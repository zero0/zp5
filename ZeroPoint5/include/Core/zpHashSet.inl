template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator>::zpHashSet()
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator_value() )
{}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( zp_size_t capacity )
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

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( comparer_const_reference cmp )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator_value() )
{}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator )
{}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( zp_size_t capacity, comparer_const_reference cmp )
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

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( zp_size_t capacity, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( comparer_value() )
    , m_allocator( allocator )
{
    ensureCapacity( capacity, false );
}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( zp_size_t capacity, comparer_const_reference cmp, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp )
    , m_allocator( allocator )
{
    ensureCapacity( capacity, false );
}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::zpHashSet( comparer_const_reference cmp, allocator_const_reference allocator )
    : m_entries( ZP_NULL )
    , m_buckets( ZP_NULL )
    , m_capacity( 0 )
    , m_count( 0 )
    , m_freeCount( 0 )
    , m_freeList( 0 )
    , m_cmp( cmp ),
    , m_allocator( allocator )
{}

template< typename T, typename Comparer, typename Allocator >
zpHashSet<T, Comparer, Allocator >::~zpHashSet()
{
    destroy();
}

template< typename T, typename Comparer, typename Allocator >
zp_bool zpHashSet<T, Comparer, Allocator >::add( const_reference value )
{
    if( m_buckets == ZP_NULL )
    {
        ensureCapacity( 4, false );
    }

    const zp_hash_t hash = m_cmp.getHash( value );
    zp_size_t index = hash % m_capacity;
    zp_size_t numSteps = 0;

    for( zp_size_t b = m_buckets[ index ]; b != npos; b = m_entries[ b ].next )
    {
        if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].value, value ) )
        {
            return false;
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
    m_entries[ newIndex ].value = zp_move( zp_forward( value ) );

    m_buckets[ index ] = newIndex;

    if( numSteps >= m_capacity )
    {
        ensureCapacity( m_capacity + 1, true );
    }

    return true;
}

template< typename T, typename Comparer, typename Allocator >
zp_bool zpHashSet<T, Comparer, Allocator >::remove( const_reference value )
{
    zp_bool removed = false;

    if( m_buckets != ZP_NULL )
    {
        const zp_hash_t hash = m_cmp.getHash( value );
        const zp_size_t index = hash % m_capacity;
        zp_size_t removeIndex = npos;

        for( zp_size_t b = m_buckets[ index ]; b != npos; b = m_entries[ b ].next )
        {
            if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].value, value ) )
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
                ( &m_entries[ b ].value )->~value_type();

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

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::unionWith( const_pointer ptr, zp_size_t count )
{
    for( zp_size_t i = 0; i < count; ++i )
    {
        add( *( ptr + i ) );
    }
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::unionWith( const_iterator begin, const_iterator end )
{
    for( ; begin != end; ++begin )
    {
        add( *begin );
    }
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::exceptWith( const_pointer ptr, zp_size_t count )
{
    for( zp_size_t i = 0; i < count; ++i )
    {
        remove( *( ptr + i ) );
    }
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::exceptWith( const_iterator begin, const_iterator end )
{
    for( ; begin != end; ++begin )
    {
        remove( *begin );
    }
}

template< typename T, typename Comparer, typename Allocator >
zp_bool zpHashSet<T, Comparer, Allocator >::contains( const_reference value ) const
{
    const zp_size_t index = findIndex( value );
    return index != npos;
}

template< typename T, typename Comparer, typename Allocator >
zp_size_t zpHashSet<T, Comparer, Allocator >::size() const
{
    return m_count - m_freeCount;
}

template< typename T, typename Comparer, typename Allocator >
zp_bool zpHashSet<T, Comparer, Allocator >::isEmpty() const
{
    return ( m_count - m_freeCount ) == 0;
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::clear()
{
    if( m_count > 0 )
    {
        for( zp_size_t i = 0; i < m_capacity; ++i )
        {
            m_buckets[ i ] = npos;
        }

        for( zp_size_t i = 0; i < m_count; ++i )
        {
            ( m_entries + i )->~zpHashSetEntry();
        }

        m_freeList = npos;
        m_freeCount = 0;
        m_count = 0;
    }
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::reserve( zp_size_t size )
{
    ensureCapacity( size, false );
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::destroy()
{
    clear();

    m_allocator.free( m_buckets );
    m_buckets = ZP_NULL;

    m_allocator.free( m_entries );
    m_entries = ZP_NULL;

    m_capacity = 0;
}

template< typename T, typename Comparer, typename Allocator >
typename zpHashSet<T, Comparer, Allocator >::iterator zpHashSet<T, Comparer, Allocator >::begin()
{
    zp_size_t index = 0;
    while( index < m_count )
    {
        if( m_entries[ index ].hash != npos )
        {
            break;
        }
        ++index;
    }

    return iterator( this, index );
}

template< typename T, typename Comparer, typename Allocator >
typename zpHashSet<T, Comparer, Allocator >::iterator zpHashSet<T, Comparer, Allocator >::end()
{
    return iterator( this, m_count );
}

template< typename T, typename Comparer, typename Allocator >
typename zpHashSet<T, Comparer, Allocator >::const_iterator zpHashSet<T, Comparer, Allocator >::begin() const
{
    zp_size_t index = 0;
    while( index < m_count )
    {
        if( m_entries[ index ].hash != npos )
        {
            break;
        }
        ++index;
    }

    return const_iterator( this, index );
}

template< typename T, typename Comparer, typename Allocator >
typename zpHashSet<T, Comparer, Allocator >::const_iterator zpHashSet<T, Comparer, Allocator >::end() const
{
    return const_iterator( this, m_count );
}

template< typename T, typename Comparer, typename Allocator >
void zpHashSet<T, Comparer, Allocator >::ensureCapacity( zp_size_t capacity, zp_bool forceRehash )
{
    if( capacity > m_capacity )
    {
        zp_size_t* buckets = static_cast<zp_size_t*>( m_allocator.allocate( sizeof( zp_size_t ) * capacity ) );
        zpHashSetEntry* entries = static_cast<zpHashSetEntry*>( m_allocator.allocate( sizeof( zpHashSetEntry ) * capacity ) );
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
                        entries[ i ].hash = m_cmp.getHash( entries[ i ].value );
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

template< typename T, typename Comparer, typename Allocator >
zp_size_t zpHashSet<T, Comparer, Allocator >::findIndex( const_reference value ) const
{
    zp_size_t index = npos;

    if( m_buckets != ZP_NULL )
    {
        const zp_hash_t hash = m_cmp.getHash( value );
        for( zp_size_t b = m_buckets[ hash % m_capacity ]; b != npos; b = m_entries[ b ].next )
        {
            if( m_entries[ b ].hash == hash && m_cmp.equals( m_entries[ b ].value, value ) )
            {
                index = b;
                break;
            }
        }
    }

    return index;
}