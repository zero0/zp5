
#include <new>

template< typename T >
T* zpVectorAllocator< T >::allocate( zp_size_t count )
{
    void* ptr = g_globalAllocator.allocate( sizeof( T ) * count );
    return static_cast<T*>( ptr );
}

template< typename T >
void zpVectorAllocator< T >::free( T* ptr )
{
    g_globalAllocator.free( ptr );
}

//
//
//

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector()
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( Allocator() )
{}

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector( const Allocator& allocator )
    : m_data( ZP_NULL )
    , m_size( 0 )
    , m_capacity( 0 )
    , m_allocator( allocator )
{}

template< typename T, typename Allocator >
zpVector< T, Allocator >::zpVector( zp_size_t capacity, const Allocator& allocator )
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
    if( m_data )
    {
        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_size = 0;
    m_capacity = 0;
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
    ensureCapacity( m_size + 1 );
    m_data[ m_size++ ] = val;
}

template< typename T, typename Allocator >
typename zpVector< T, Allocator >::reference zpVector< T, Allocator >::pushBackEmpty()
{
    ensureCapacity( m_size + 1 );
    new ( m_data + m_size ) T();
    return m_data[ m_size++ ];
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::pushFront( const_reference val )
{
    ensureCapacity( m_size + 1 );

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
    ensureCapacity( m_size + 1 );

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
void zpVector< T, Allocator >::erase( zp_size_t index )
{
    ( m_data + index )->~T();
    m_data[ index ] = zp_move( zp_forward( m_data[ m_size - 1 ] ) );
    --m_size;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::eraseAll( const T& val )
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
    ensureCapacity( size );
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::destroy()
{
    if( m_data )
    {
        m_allocator.free( m_data );
        m_data = ZP_NULL;
    }

    m_size = 0;
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
void zpVector< T, Allocator >::ensureCapacity( zp_size_t size )
{
    if( m_capacity < size )
    {
        if( m_capacity == 0 )
        {
            m_capacity = size < 4 ? 4 : size;
        }
        else
        {
            while( m_capacity < size )
            {
                m_capacity *= 2;
            }
        }

        pointer newArray = m_allocator.allocate( m_capacity );
        zp_memset( newArray, 0, m_capacity * sizeof( T ) );

        if( m_data )
        {
            //if( m_size )
            //{
            //    zp_memcpy( newArray, sizeof( T ) * m_capacity, m_data, sizeof( T ) * m_size );
            //}

            for( zp_size_t i = 0; i != m_size; ++i )
            {
                newArray[ i ] = zp_move( zp_forward( m_data[ i ] ) );
            }

            m_allocator.free( m_data );
        }

        m_data = newArray;
    }
}
