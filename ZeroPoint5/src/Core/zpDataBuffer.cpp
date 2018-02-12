#include "Core/zpCore.h"

zp_byte* zpDataBufferAllocator::allocate( zp_size_t size )
{
    void* data = g_globalAllocator.allocate( size );
    return static_cast<zp_byte*>( data );
}

void zpDataBufferAllocator::free( zp_byte* ptr )
{
    g_globalAllocator.free( ptr );
}

zpDataBuffer::zpDataBuffer()
    : m_buffer( ZP_NULL )
    , m_position( 0 )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_isFixed( false )
    , m_allocator( zpDataBufferAllocator() )
{

}

zpDataBuffer::zpDataBuffer( zp_size_t capacity )
    : m_buffer( ZP_NULL )
    , m_position( 0 )
    , m_length( 0 )
    , m_capacity( 0 )
    , m_isFixed( false )
    , m_allocator( zpDataBufferAllocator() )
{
    ensureCapacity( capacity );
}

zpDataBuffer::zpDataBuffer( zp_byte* data, zp_size_t capacity )
    : m_buffer( data )
    , m_position( 0 )
    , m_length( 0 )
    , m_capacity( capacity )
    , m_isFixed( true )
    , m_allocator( zpDataBufferAllocator() )
{

}

zpDataBuffer::~zpDataBuffer()
{
    if( !m_isFixed && m_buffer )
    {
        m_allocator.free( m_buffer );
        m_buffer = ZP_NULL;
    }
}

const zp_byte* zpDataBuffer::getData() const
{
    return m_buffer + m_position;
}

zp_byte* zpDataBuffer::getData()
{
    return m_buffer + m_position;
}

const zp_byte* zpDataBuffer::getBuffer() const
{
    return m_buffer;
}

zp_byte* zpDataBuffer::getBuffer()
{
    return m_buffer;
}

zp_size_t zpDataBuffer::getPosition() const
{
    return m_position;
}

zp_size_t zpDataBuffer::getLength() const
{
    return m_length;
}

zp_size_t zpDataBuffer::getCapacity() const
{
    return m_capacity;
}

void zpDataBuffer::clear()
{
    m_position = 0;
    m_length = 0;
}

void zpDataBuffer::reset()
{
    m_position = 0;
}

void zpDataBuffer::reserve( zp_size_t capacity )
{
    if( !m_isFixed )
    {
        ensureCapacity( capacity );
    }
}

zp_size_t zpDataBuffer::write( const void* data, zp_size_t offset, zp_size_t length )
{
    //ZP_PROFILER_BLOCK();

    if( m_isFixed )
    {
        ZP_ASSERT( ( m_position + length ) < m_capacity, "" );
    }
    else
    {
        ensureCapacity( m_length + length );
    }

    const zp_byte* d = static_cast<const zp_byte*>( data ) + offset;
    //const zp_byte* e = d + length;
    //zp_byte* b = m_buffer + m_position;

    //for( ; d != e; ++b, ++d )
    //{
    //    *b = *d;
    //}
    zp_char* buff = reinterpret_cast<zp_char*>( m_buffer + m_position );
    zp_size_t len = length;
    //if( len >= 128 )
    //{
    //    __m128i mask = _mm_set1_epi32( 0xffffffff );
    //    while( len >= 128 )
    //    {
    //        __m128i data = _mm_loadu_si128( reinterpret_cast<const __m128i*>( d ) );
    //        _mm_maskmoveu_si128( data, mask, buff );
    //
    //        len -= 128;
    //        buff += 128;
    //        d += 128;
    //    }
    //}

    if( len >= 4 )
    {
        zp_int* ibuff = (zp_int*)( buff );
        const zp_int* id = reinterpret_cast<const zp_int*>( d );
        do
        {
            _mm_stream_si32( ibuff, *id );

            ++ibuff;
            ++id;
            len -= 4;
        } while( len >= 4 );
    }

    if( len > 0 )
    {
        zp_memcpy( buff, m_capacity - m_position, d, len );
    }

    m_position += length;
    m_length += length;

    return length;
}

zp_size_t zpDataBuffer::read( void* data, zp_size_t offset, zp_size_t length )
{
    zp_byte* d = static_cast<zp_byte*>( data );

    zp_size_t bytesRead = length;
    if( ( m_position + length ) > m_length )
    {
        bytesRead = m_length - m_position;
    }

    zp_memcpy( d + offset, length, m_buffer + m_position, bytesRead );

    m_position += bytesRead;

    return bytesRead;
}

void zpDataBuffer::ensureCapacity( zp_size_t capacity )
{
    if( m_buffer )
    {
        if( capacity > m_capacity )
        {
            zp_byte* newBuffer = m_allocator.allocate( capacity );
            
            zp_memcpy( newBuffer, capacity, m_buffer, m_length );

            m_allocator.free( m_buffer );
            
            m_buffer = newBuffer;
            m_capacity = capacity;
        }
    }
    else
    {
        m_buffer = m_allocator.allocate( capacity );
        m_capacity = capacity;
    }
}