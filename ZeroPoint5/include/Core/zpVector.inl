
//#include <new>

//
//
//

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector()
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
}

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector( allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
}

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector( zp_size_t capacity )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator_value() )
{
    ensureCapacity( capacity );
}

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector( zp_size_t capacity, allocator_const_reference allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{
    ensureCapacity( capacity );
}

template< typename T, typename Allocator >
zpVector< T, Allocator >::~zpVector()
{
    destroy();
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::operator[]( zp_size_t index )
{
    return m_data[ index ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_reference zpVector< T, Allocator >::operator[]( zp_size_t index ) const
{
    return m_data[ index ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_reference zpVector< T, Allocator >::at( zp_size_t index ) const
{
    return m_data[ index ];
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::size() const
{
    return m_size;
}

template< typename T, typename Allocator >
zp_bool zpVector< T, Allocator >::isEmpty() const
{
    return m_size == 0;
}

template< typename T, typename Allocator >
zp_bool zpVector< T, Allocator >::isFixed() const
{
    return false;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::pushBack( const_reference val )
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }
    m_data[ m_size++ ] = val;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::pushBackEmpty()
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }
    new ( m_data + m_size ) T();
    return m_data[ m_size++ ];
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::pushFront( const_reference val )
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }

    for( zp_size_t i = m_size + 1; i >= 1; --i )
    {
        m_data[ i ] = zp_move( zp_forward( m_data[ i - 1 ] ) );
    }

    ++m_size;

    m_data[ 0 ] = val;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::pushFrontEmpty()
{
    if( m_size == m_capacity )
    {
        ensureCapacity( m_capacity * 2 );
    }

    for( zp_size_t i = m_size + 1; i >= 1; --i )
    {
        m_data[ i ] = zp_move( zp_forward( m_data[ i - 1 ] ) );
    }

    ++m_size;

    new ( m_data ) T();
    return m_data[ 0 ];
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::popBack()
{
    if( m_size )
    {
        ( m_data + --m_size )->~T();
    }
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::popFront()
{
    if( m_size )
    {
        m_data->~T();

        for( zp_size_t i = 1; i < m_size; ++i )
        {
            m_data[ i - 1 ] = zp_move( zp_forward( m_array[ i ] ) );
        }

        --m_size;
    }
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::eraseAt( zp_size_t index )
{
    ( m_data + index )->~T();
    m_data[ index ] = zp_move( zp_forward( m_data[ m_size - 1 ] ) );
    --m_size;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::eraseAtSwapBack( zp_size_t index )
{
    ( m_data + index )->~T();
    m_data[ index ] = zp_move( zp_forward( m_data[ m_size - 1 ] ) );
    --m_size;
}

template< typename T, typename Allocator >
zp_bool zpVector< T, Allocator >::erase( const_reference val )
{
    zp_size_t index = indexOf( val );
    zp_bool found = index != npos;

    if( found )
    {
        eraseAt( index );
    }

    return found;
}

template< typename T, typename Allocator >
zp_bool zpVector< T, Allocator >::eraseSwapBack( const_reference val )
{
    zp_size_t index = indexOf( val );
    zp_bool found = index != npos;

    if( found )
    {
        eraseAtSwapBack( index );
    }

    return found;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::eraseAll( const_reference val )
{
    zp_size_t numErased = 0;
    for( zp_size_t i = 0; i < m_size; ++i )
    {
        T* t = m_data + i;
        if( *t == val )
        {
            ++numErased;

            t->~T();

            m_data[ i ] = zp_move( zp_forward( m_data[ m_size - 1 ] ) );
            --m_size;
            --i;
        }

    }
    return numErased;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::clear()
{
    iterator b = m_data;
    iterator e = m_data + m_size;
    for( ; b != e; ++b )
    {
        b->~T();
    }

    m_size = 0;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::reset()
{
    m_size = 0;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::reserve( zp_size_t size )
{
    if( size > m_capacity )
    {
        ensureCapacity( size );
    }
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::destroy()
{
    clear();

    if( m_data )
    {
        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_capacity = 0;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::indexOf( const_reference val ) const
{
    zp_size_t index = npos;

    const_iterator b = m_data;
    const_iterator e = m_data + m_size;
    for( ; b != e; ++b )
    {
        if( *b == val )
        {
            index = b - m_data;
            break;
        }

    }

    return index;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::lastIndexOf( const_reference val ) const
{
    zp_size_t index = npos;

    const_iterator b = m_data;
    const_iterator e = m_data + m_size;
    for( ; b != e; )
    {
        --e;
        if( *e == val )
        {
            index = e - m_data;
            break;
        }
    }

    return index;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::front()
{
    return m_data[ 0 ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::back()
{
    return m_data[ m_size - 1 ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_reference zpVector< T, Allocator >::front() const
{
    return m_data[ 0 ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_reference zpVector< T, Allocator >::back() const
{
    return m_date[ m_size - 1 ];
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::iterator zpVector< T, Allocator >::begin()
{
    return m_data;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::iterator zpVector< T, Allocator >::end()
{
    return m_data + m_size;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_iterator zpVector< T, Allocator >::begin() const
{
    return m_data;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::const_iterator zpVector< T, Allocator >::end() const
{
    return m_data + m_size;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::ensureCapacity( zp_size_t capacity )
{
    capacity = ZP_MAX( capacity, 4 );

    pointer newArray = static_cast<pointer>( m_allocator.allocate( sizeof( T ) * capacity ) );
    zp_zero_memory_array( newArray, capacity );

    if( m_data != ZP_NULL )
    {
        for( zp_size_t i = 0; i != m_size; ++i )
        {
            newArray[ i ] = zp_move( zp_forward( m_data[ i ] ) );
        }

        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_data = newArray;
    m_capacity = capacity;
}
