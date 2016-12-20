
thread_local static zpBlockAllocator g_vectorAllocator( ZP_MEMORY_MB( 4 ) );

template< typename T >
T* zpVectorAllocator< T >::allocate( zp_size_t count )
{
    void* ptr = g_vectorAllocator.allocate( sizeof( T ) * count );
    return static_cast<T*>( ptr );
}

template< typename T >
void zpVectorAllocator< T >::free( T* ptr )
{
    g_vectorAllocator.free( ptr );
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
T& zpVector< T, Allocator >::operator[]( zp_size_t index )
{
    return m_data[ index ];
}

template< typename T, typename Allocator >
const T& zpVector< T, Allocator >::operator[]( zp_size_t index ) const
{
    return m_data[ index ];
}

template< typename T, typename Allocator >
const T& zpVector< T, Allocator >::at( zp_size_t index ) const
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
void zpVector< T, Allocator >::pushBack( const T& val )
{
    ensureCapacity( m_size + 1 );
    m_data[ m_size++ ] = val;
}

template< typename T, typename Allocator >
T& zpVector< T, Allocator >::pushBackEmpty()
{
    ensureCapacity( m_size + 1 );
    m_data[ m_size++ ] = (T&&)T();
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::pushFront( const T& val )
{
    ensureCapacity( m_size + 1 );

    for( zp_size_t i = m_size + 1; i >= 1; --i )
    {
        m_array[ i ] = (T&&)m_array[ i - 1 ];
    }

    m_array[ 0 ] = val;

    ++m_size;
}

template< typename T, typename Allocator >
T& zpVector< T, Allocator >::pushFrontEmpty()
{
    ensureCapacity( m_size + 1 );

    for( zp_size_t i = m_size + 1; i >= 1; --i )
    {
        m_array[ i ] = (T&&)m_array[ i - 1 ];
    }

    m_array[ 0 ] = (T&&)T();

    ++m_size;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::popBack()
{
    if( m_size )
    {
        ( m_array + --m_size )->~T();
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
            m_data[ i - 1 ] = (T&&)m_array[ i ];
        }

        --m_size;
    }
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::erase( zp_size_t index )
{
    ( m_data + index )->~T();
    m_data[ index ] = (T&&)m_data[ m_size - 1 ];
    --m_size;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::eraseAll( const T& val )
{
    zp_size_t numErased = 0;
    return numErased;
}

template< typename T, typename Allocator >
void zpVector< T, Allocator >::clear()
{
    T* b = m_data;
    T* e = m_data + m_size;
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
zp_size_t zpVector< T, Allocator >::indexOf( const T& val ) const
{
    zp_size_t index = npos;
    return index;
}

template< typename T, typename Allocator >
zp_size_t zpVector< T, Allocator >::lastIndexOf( const T& val ) const
{
    zp_size_t index = npos;
    return index;
}

template< typename T, typename Allocator >
T& zpVector< T, Allocator >::front()
{
    return m_data[ 0 ];
}

template< typename T, typename Allocator >
T& zpVector< T, Allocator >::back()
{
    return m_date[ m_size - 1 ];
}

template< typename T, typename Allocator >
const T& zpVector< T, Allocator >::front() const
{
    return m_data[ 0 ];
}

template< typename T, typename Allocator >
const T& zpVector< T, Allocator >::back() const
{
    return m_date[ m_size - 1 ];
}

template< typename T, typename Allocator >
T* zpVector< T, Allocator >::begin()
{
    return m_data;
}

template< typename T, typename Allocator >
T* zpVector< T, Allocator >::end()
{
    return m_data + m_size;
}

template< typename T, typename Allocator >
const T* zpVector< T, Allocator >::begin() const
{
    return m_data;
}

template< typename T, typename Allocator >
const T* zpVector< T, Allocator >::end() const
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

        T* newArray = m_allocator.allocate( m_capacity );

        if( m_data )
        {
            for( zp_size_t i = 0; i != m_size; ++i )
            {
                newArray[ i ] = (T&&)m_data[ i ];
            }

            m_allocator.free( m_data );
        }

        m_data = newArray;
    }
}
