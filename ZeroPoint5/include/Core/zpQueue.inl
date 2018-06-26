
template<typename T, typename Allocator>
zpQueue<T, Allocator>::zpQueue()
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_head( 0 )
    , m_tail( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
}

template<typename T, typename Allocator>
zpQueue<T, Allocator>::zpQueue( allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_head( 0 )
    , m_tail( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
}

template<typename T, typename Allocator>
zpQueue<T, Allocator>::zpQueue( zp_size_t capacity )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_head( 0 )
    , m_tail( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
    ensureCapacity( capacity );
}

template<typename T, typename Allocator>
zpQueue<T, Allocator>::zpQueue( zp_size_t capacity, allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_head( 0 )
    , m_tail( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
    ensureCapacity( capacity );
}

template<typename T, typename Allocator>
zpQueue<T, Allocator>::~zpQueue()
{
    destroy();
}

template<typename T, typename Allocator>
zp_size_t zpQueue<T, Allocator>::size() const
{
    return m_size;
}

template<typename T, typename Allocator>
zp_bool zpQueue<T, Allocator>::isEmpty() const
{
    return m_size == 0;
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::enqueue( const_reference val )
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }
    m_data[ m_tail ] = val;
    m_tail = ( m_tail + 1 ) % m_capacity;
    ++m_size;
}

template<typename T, typename Allocator>
typename zpQueue<T, Allocator>::value_type zpQueue<T, Allocator>::dequeue()
{
    ZP_ASSERT( m_size, "Trying to dequeue from an empty Queue" );

    pointer p = m_data + m_head;

    value_type v = zp_move( zp_forward( *p ) );
    p->~T();

    m_head = ( m_head + 1 ) % m_capacity;
    --m_size;

    return v;
}

template<typename T, typename Allocator>
zp_bool zpQueue<T, Allocator>::tryDequeue( reference val )
{
    if( m_size > 0 )
    {
        pointer p = m_data + m_head;

        val = zp_move( zp_forward( *p ) );
        p->~T();

        m_head = ( m_head + 1 ) % m_capacity;
        --m_size;

        return true;
    }
    return false;
}

template<typename T, typename Allocator>
typename zpQueue<T, Allocator>::const_reference zpQueue<T, Allocator>::peek() const
{
    return m_data[ m_tail ];
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::clear()
{
    if( m_head < m_tail )
    {
        for( zp_size_t i = m_head, imax = m_tail; i < imax; ++i )
        {
            ( m_data + i )->~T();
        }
    }
    else if( m_size > 0 )
    {
        for( zp_size_t i = m_head, imax = m_capacity; i < imax; ++i )
        {
            ( m_data + i )->~T();
        }
        for( zp_size_t i = 0, imax = m_tail; i < imax; ++i )
        {
            ( m_data + i )->~T();
        }
    }

    m_head = 0;
    m_tail = 0;
    m_size = 0;
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::reset()
{
    m_head = 0;
    m_tail = 0;
    m_size = 0;
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::reserve( zp_size_t size )
{
    if( capacity > m_capacity )
    {
        ensureCapacity( size );
    }
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::destroy()
{
    clear();

    if( m_data != ZP_NULL )
    {
        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_capacity = 0;
}

template<typename T, typename Allocator>
void zpQueue<T, Allocator>::ensureCapacity( zp_size_t capacity )
{
    capacity = ZP_MAX( capacity, 4 );

    pointer newData = static_cast<pointer>( m_allocator.allocate( sizeof( T ) * capacity ) );
    zp_zero_memory_array( newData, capacity );

    if( m_data != ZP_NULL )
    {
        if( m_head < m_tail )
        {
            for( zp_size_t i = m_head, imax = m_tail, n = 0; i < imax; ++i, ++n )
            {
                newData[ n ] = zp_move( zp_forward( m_data[ i ] ) );
            }
        }
        else if( m_size > 0 )
        {
            zp_size_t n = 0;
            for( zp_size_t i = m_head, imax = m_capacity; i < imax; ++i, ++n )
            {
                newData[ n ] = zp_move( zp_forward( m_data[ i ] ) );
            }
            for( zp_size_t i = 0, imax = m_tail; i < imax; ++i, ++n )
            {
                newData[ n ] = zp_move( zp_forward( m_data[ i ] ) );
            }
        }

        m_head = 0;
        m_tail = m_size;

        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_data = newData;
    m_capacity = capacity;
}
