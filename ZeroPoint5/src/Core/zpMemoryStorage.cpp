#include "Core/zpCore.h"

zpDirectMemoryStorage::zpDirectMemoryStorage()
    : m_memory( ZP_NULL )
    , m_size( 0 )
{
}
zpDirectMemoryStorage::zpDirectMemoryStorage( void* memory, zp_size_t size )
    : m_memory( memory )
    , m_size( size )
{
}

zpDirectMemoryStorage::~zpDirectMemoryStorage()
{
    m_memory = ZP_NULL;
    m_size = 0;
}

void* zpDirectMemoryStorage::getMemory()
{
    return m_memory;
}

zp_size_t zpDirectMemoryStorage::getTotalMemory() const
{
    return m_size;
}

    