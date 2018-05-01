#include "Core/zpCore.h"

zpTLFSMemoryPolicy::zpTLFSMemoryPolicy()
    : m_tlsf( ZP_NULL )
    , m_totalMemoryUsed( 0 )
{
}
zpTLFSMemoryPolicy::~zpTLFSMemoryPolicy()
{
}

void zpTLFSMemoryPolicy::setup( void* memory, zp_size_t size )
{
    m_tlsf = zp_tlsf_create_with_pool( memory, size );
}

void zpTLFSMemoryPolicy::teardown()
{
    zp_tlsf_remove_pool( m_tlsf, zp_tlsf_get_pool( m_tlsf ) );
    zp_tlsf_destroy( m_tlsf );
    m_tlsf = ZP_NULL;
}

void* zpTLFSMemoryPolicy::allocate( zp_size_t size )
{
    void* ptr;

    ptr = zp_tlsf_malloc( m_tlsf, size );
    m_totalMemoryUsed += zp_tlsf_block_size( ptr );

    return ptr;
}

void zpTLFSMemoryPolicy::free( void* ptr )
{
    m_totalMemoryUsed -= zp_tlsf_block_size( ptr );
    zp_tlsf_free( m_tlsf, ptr );
}

zp_size_t zpTLFSMemoryPolicy::getMemoryUsed() const
{
    return m_totalMemoryUsed;
}
