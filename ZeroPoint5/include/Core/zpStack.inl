
template<typename T, typename Allocator>
zpStack<T, Allocator>::zpStack()
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
}

template<typename T, typename Allocator>
zpStack<T, Allocator>::zpStack( allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
}

template<typename T, typename Allocator>
zpStack<T, Allocator>::zpStack( zp_size_t capacity )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
    ensureCapacity( capacity );
}

template<typename T, typename Allocator>
zpStack<T, Allocator>::zpStack( zp_size_t capacity, allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
    ensureCapacity( capacity );
}

template<typename T, typename Allocator>
zpStack<T, Allocator>::~zpStack()
{
    destroy();
}

template<typename T, typename Allocator>
zp_size_t zpStack<T, Allocator>::size() const
{
    return m_size;
}

template<typename T, typename Allocator>
zp_bool zpStack<T, Allocator>::isEmpty() const
{
    return m_size == 0;
}

template<typename T, typename Allocator>
void zpStack<T, Allocator>::push( const_reference val )
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }

    m_data[ m_size++ ] = val;
}

template<typename T, typename Allocator>
typename zpStack<T, Allocator>::value_type zpStack<T, Allocator>::pop()
{
    ZP_ASSERT( m_size, "Trying to pop from empty Stack" );

    pointer p = m_data + ( m_size - 1 );

    value_type v = zp_move( zp_forward( *p ) );
    p->~T();

    --m_size;

    return zp_move( zp_forward( v ) );
}

template<typename T, typename Allocator>
zp_bool zpStack<T, Allocator>::tryPop( reference val )
{
    if( m_size > 0 )
    {
        pointer p = m_data + ( m_size - 1 );

        val = zp_move( zp_forward( *p ) );
        p->~T();

        --m_size;

        return true;
    }

    return false;
}

template<typename T, typename Allocator>
typename zpStack<T, Allocator>::const_reference zpStack<T, Allocator>::peek() const
{
    return m_data[ m_size ];
}

template<typename T, typename Allocator>
void zpStack<T, Allocator>::clear()
{
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        ( m_data + i )->~T();
    }

    m_size = 0;
}

template<typename T, typename Allocator>
void zpStack<T, Allocator>::reset()
{
    m_size = 0;
}

template<typename T, typename Allocator>
void zpStack<T, Allocator>::reserve( zp_size_t size )
{
    if( size > m_capacity )
    {
        ensureCapacity( size );
    }
}

template<typename T, typename Allocator>
void zpStack<T, Allocator>::destroy()
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
void zpStack<T, Allocator>::ensureCapacity( zp_size_t capacity )
{
    capacity = ZP_MAX( 4, capacity );

    pointer newData = static_cast<pointer>( m_allocator.allocate( sizeof( T ) * capacity ) );
    zp_zero_memory_array( newData, capacity );

    if( m_data != ZP_NULL )
    {
        for( zp_size_t i = 0; i < m_size; ++i )
        {
            newData[ i ] = zp_move( zp_forward( m_data[ i ] ) );
        }

        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_data = newData;
    m_capacity = capacity;
}
