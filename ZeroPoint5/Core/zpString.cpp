#include "zpCore.h"

thread_local static zpBlockAllocator g_stringAllocator( ZP_MEMORY_MB( 1 ) );

zp_char* zpStringAllocator::allocate( zp_size_t size )
{
    void* str = g_stringAllocator.allocate( size );
    return static_cast<zp_char*>( str );
}
void zpStringAllocator::free( zp_char* ptr )
{
    g_stringAllocator.free( ptr );
}


zpString::zpString()
    : m_str( ZP_NULL )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_alloc()
{}

zpString::zpString( const zp_char* str )
    : m_str( ZP_NULL )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_alloc()
{
    assign( str );
}

zpString::zpString( const zp_char* str, zp_size_t length )
    : m_str( ZP_NULL )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_alloc()
{
    assign( str, length );
}

zpString::zpString( const zpString& other )
    : m_str( ZP_NULL )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_alloc()
{
    assign( other.m_str, other.m_length );
}

zpString::zpString( zpString&& other )
    : m_str( other.m_str )
    , m_length( other.m_length )
    , m_capacity( other.m_capacity )
    , m_alloc()
{
    other.m_str = ZP_NULL;
    other.m_length = 0;
    other.m_capacity = 0;
}

zpString::~zpString()
{
    if( m_str )
    {
        m_alloc.free( m_str );
        m_str = ZP_NULL;
    }

    m_length = 0;
    m_capacity = 0;
}

zpString& zpString::operator=( const zpString& other )
{
    if( this != &other )
    {
        assign( other.m_str, other.m_length );
    }

    return *this;
}
zpString& zpString::operator=( zpString&& other )
{
    if( this != &other )
    {
        m_str = other.m_str;
        m_length = other.m_length;
        m_capacity = other.m_capacity;

        other.m_str = ZP_NULL;
        other.m_length = 0;
        other.m_capacity = 0;
    }

    return *this;
}
zpString& zpString::operator=( const zp_char* other )
{
    assign( other );

    return *this;
}

const zp_char* zpString::str() const
{
    return m_str;
}

zp_size_t zpString::length() const
{
    return m_length;
}

zp_size_t zpString::capacity() const
{
    return m_capacity;
}

zp_bool zpString::isEmpty() const
{
    return m_length == 0;
}

zp_char& zpString::operator[]( zp_size_t index )
{
    return m_str[ index ];
}

const zp_char& zpString::operator[]( zp_size_t index ) const
{
    return m_str[ index ];
}

zp_char& zpString::at( zp_size_t index )
{
    ZP_ASSERT( index < m_length, "" );
    return m_str[ index ];
}

const zp_char& zpString::at( zp_size_t index ) const
{
    ZP_ASSERT( index < m_length, "" );
    return m_str[ index ];
}

zp_char& zpString::front()
{
    return m_str[ 0 ];
}

const zp_char& zpString::front() const
{
    return m_str[ 0 ];
}

zp_char& zpString::back()
{
    return m_str[ m_length - 1 ];
}

const zp_char& zpString::back() const
{
    return m_str[ m_length - 1 ];
}

zp_char* zpString::begin()
{
    return m_str;
}

const zp_char* zpString::begin() const
{
    return m_str;
}

zp_char* zpString::end()
{
    return m_str + m_length;
}

const zp_char* zpString::end() const
{
    return m_str + m_length;
}

void zpString::clear()
{
    m_length = 0;
    if( m_str ) m_str[ 0 ] = '\0';
}

void zpString::reserve( zp_size_t size )
{
    if( m_capacity < size )
    {
        zp_char* newStr = m_alloc.allocate( size );

        if( m_str )
        {
            zp_strncpy( newStr, m_length, m_str, m_length );
            m_alloc.free( m_str );
        }

        m_capacity = size;
        m_str = newStr;
    }
}

void zpString::assign( const zp_char* str )
{
    zp_size_t length = zp_strlen( str );
    assign( str, length );
}

void zpString::assign( const zp_char* str, zp_size_t length )
{
    if( m_capacity < length )
    {
        m_capacity = length + 1;
        zp_char* newStr = m_alloc.allocate( m_capacity );

        if( m_str )
        {
            m_alloc.free( m_str );
        }

        m_str = newStr;
    }

    m_length = length;
    zp_strncpy( m_str, m_capacity, str, length );
}

zpString zpString::substr( zp_size_t pos, zp_size_t count ) const
{
    ZP_ASSERT( pos < m_length, "" );
    if( count == npos || pos + count > m_length )
    {
        count = m_length - pos;
    }

    return zpString( m_str + pos, count );
}

