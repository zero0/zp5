
template< zp_size_t Size, zp_size_t StackBlocks = 32 >
zpStackAllocator<Size, StackBlocks>::zpStackAllocator()
    : m_memUsed( 0 )
    , m_stackCount( 0 )
{}

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
zpStackAllocator<Size, StackBlocks>::~zpStackAllocator()
{
    ZP_ASSERT( m_memUsed == 0, "" );
    ZP_ASSERT( m_stackCount == 0, "" );
}

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
void* zpStackAllocator<Size, StackBlocks>::allocate( zp_size_t size )
{
    ZP_ASSERT( m_stackCount != StackBlocks, "" );
    ZP_ASSERT( ( m_memUsed + size ) < Size, "" );

    zpStackBlock* block = m_stackBlocks + m_stackCount;
    block->size = size;
    block->data = m_data + m_memUsed;

    m_memUsed += size;
    ++m_stackCount;

    return block->data;
}

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
void zpStackAllocator<Size, StackBlocks>::free( void* ptr )
{
    ZP_ASSERT( m_stackCount > 0, "" );

    zpStackBlock* block = m_stackBlocks + m_stackCount - 1;
    ZP_ASSERT( block->data == ptr, "" );

    m_memUsed -= block->size;
    --m_stackCount;
}

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
zp_size_t zpStackAllocator<Size, StackBlocks>::getMemoryUsed() const
{
    return m_memUsed;
}

template< zp_size_t Size, zp_size_t StackBlocks = 32 >
zp_size_t zpStackAllocator<Size, StackBlocks>::getTotalMemory() const
{
    return Size;
}
