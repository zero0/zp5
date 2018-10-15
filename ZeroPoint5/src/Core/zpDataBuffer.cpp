#include "Core/zpCore.h"

zp_byte* zpDataBufferAllocator::allocate( zp_size_t size )
{
    void* data = g_globalAllocator->allocate( size );
    return static_cast<zp_byte*>( data );
}

void zpDataBufferAllocator::free( zp_byte* ptr )
{
    g_globalAllocator->free( ptr );
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
    if( !m_isFixed && capacity > m_capacity )
    {
        ensureCapacity( capacity );
    }
}

zp_size_t zpDataBuffer::write( const void* data, zp_size_t offset, zp_size_t length )
{
    //ZP_PROFILER_BLOCK();
    const zp_size_t newLength = length + m_position;
    if( m_isFixed )
    {
        ZP_ASSERT( ( m_position + length ) < m_capacity, "" );
    }
    else if( newLength > m_capacity )
    {
        ensureCapacity( ZP_MAX( newLength, m_capacity * 2 ) );
    }


    const zp_byte* d = static_cast<const zp_byte*>( data ) + offset;
    zp_memcpy( m_buffer + m_position, m_capacity - m_position, d, length );

    m_position += length;
    m_length += length;

    return length;

#if 0
    //const zp_byte* e = d + length;
    //zp_byte* b = m_buffer + m_position;

    //for( ; d != e; ++b, ++d )
    //{
    //    *b = *d;
    //}
    zp_byte* buff = ( m_buffer + m_position );
    zp_size_t len = length;
    
    if( len >= 16 )
    {
        __m128i mask = _mm_set1_epi32( 0xffffffff );        
        zp_char* mbuff = reinterpret_cast<zp_char*>( buff );
        const __m128i* md = reinterpret_cast<const __m128i*>( d );

        for( ; len >= 16; mbuff += 16, ++md, buff += 16, d += 16, len -= 16 )
        {
            __m128i mdata = _mm_loadu_si128( md );
            _mm_maskmoveu_si128( mdata, mask, mbuff );
        }
    }

    //if( len >= 8 )
    //{
    //    zp_long* lbuff = reinterpret_cast<zp_long*>( buff );
    //    const zp_long* ld = reinterpret_cast<const zp_long*>( d );
    //    do
    //    {
    //        _mm_stream_si64( lbuff, *ld );
    //
    //        ++lbuff;
    //        ++ld;
    //        len -= 8;
    //    } while( len >= 8 );
    //}

    if( len >= 8 )
    {
        zp_long* lbuff = reinterpret_cast<zp_long*>( buff );
        const zp_long* ld = reinterpret_cast<const zp_long*>( d );
        for( ; len >= 8; ++lbuff, ++ld, buff += 8, d += 8, len -= 8 )
        {
            *lbuff = *ld;
        }
    }

    if( len >= 4 )
    {
        zp_int* ibuff = reinterpret_cast<zp_int*>( buff );
        const zp_int* id = reinterpret_cast<const zp_int*>( d );
        for( ; len >= 4; ++ibuff, ++id, buff += 4, d += 4, len -= 4 )
        {
            _mm_stream_si32( ibuff, *id );
        }
        
        _mm_mfence();
    }

    if( len > 0 )
    {
        //zp_memcpy( buff, m_capacity - m_position, d, len );
        for( ; len > 0; ++buff, ++d, --len )
        {
            *buff = *d;
        }
    }

    m_position += length;
    m_length += length;

    return length;
#endif
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
    zp_byte* newBuffer = m_allocator.allocate( capacity );

    if( m_buffer != ZP_NULL )
    {
        zp_memcpy( newBuffer, capacity, m_buffer, m_length );

        m_allocator.free( m_buffer );
        m_buffer = ZP_NULL;
    }

    m_buffer = newBuffer;
    m_capacity = capacity;
}
