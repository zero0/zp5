#include "zpCore.h"

enum : zp_uint
{
    ZP_BLOCK_ALLOCATOR_START_CANARY =     0xCAFEBABE,
    ZP_BLOCK_ALLOCATOR_END_CANARY =       0xDEADBEEF,

    ZP_BLOCK_ALLOCATOR_INCREMENT_SHIFT =    6,
    ZP_BLOCK_ALLOCATOR_INCREMENT_SIZE =     ( 1 << ZP_BLOCK_ALLOCATOR_INCREMENT_SHIFT ),
    ZP_BLOCK_ALLOCATOR_INCREMENT_MASK =     ( ZP_BLOCK_ALLOCATOR_INCREMENT_SIZE - 1 )
};

#define ZP_BLOCK_ALLOCATOR_ALIGN_SIZE( s )  ( ( (s) + ZP_BLOCK_ALLOCATOR_INCREMENT_SIZE ) & ( ~ZP_BLOCK_ALLOCATOR_INCREMENT_MASK ) )

ZP_FORCE_INLINE void _insertAfter( zpMemoryBlock* block, zpMemoryBlock* newBlock )
{
    newBlock->next = block->next;
    newBlock->prev = block;

    block->next->prev = newBlock;
    block->next = newBlock;
}

ZP_FORCE_INLINE void _insertBefore( zpMemoryBlock* node, zpMemoryBlock* newBlock )
{
    _insertAfter( node->prev, newBlock );
}

ZP_FORCE_INLINE void _removeBlock( zpMemoryBlock* node )
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
}

ZP_FORCE_INLINE zpMemoryChunk* _createChunk( zp_size_t size )
{
    zp_size_t memSize = size + sizeof( zpMemoryChunk ) + sizeof( zpMemoryBlock ) + sizeof( zp_uint );
    zp_size_t alignedSize = ZP_BLOCK_ALLOCATOR_ALIGN_SIZE( memSize );
    void* memoryBase = zp_malloc( alignedSize );

    const zp_uint canary = ZP_BLOCK_ALLOCATOR_END_CANARY;
    zp_memcpy( static_cast<zp_byte*>( memoryBase ) + ( size + sizeof( zpMemoryChunk ) + sizeof( zpMemoryBlock ) ), sizeof( zp_uint ), &canary, sizeof( zp_uint ) );

    zpMemoryChunk* chunk = reinterpret_cast<zpMemoryChunk*>( memoryBase );
    chunk->size = size;
    chunk->alignedSize = alignedSize;
    chunk->next = ZP_NULL;
    chunk->prev = ZP_NULL;

    zpMemoryBlock* block = reinterpret_cast<zpMemoryBlock*>( static_cast<zp_byte*>( memoryBase ) + sizeof( zpMemoryChunk ) );
    block->size = 0;
    block->alignedSize = alignedSize - sizeof( zpMemoryChunk );
    block->next = block;
    block->prev = block;

    return chunk;
}

ZP_FORCE_INLINE void _freeChunk( zpMemoryChunk* chunk )
{
    void* memoryBase = static_cast<void*>( chunk );

    const zp_uint canary = ZP_BLOCK_ALLOCATOR_END_CANARY;
    zp_int cmp = zp_memcmp( static_cast<zp_byte*>( memoryBase ) + ( chunk->size + sizeof( zpMemoryChunk ) + sizeof( zpMemoryBlock ) ), &canary, sizeof( zp_uint ) );
    ZP_ASSERT( cmp == 0, "Buffer overrun" );

    zp_free( memoryBase );
}

zpBlockAllocator::zpBlockAllocator( zp_size_t minChunckSize )
    : m_minChunkSize( minChunckSize )
    , m_numAllocs( 0 )
    , m_totalMemoryUsed( 0 )
    , m_totalMemory( 0 )
{
    m_memoryChunkHead.size = 0;
    m_memoryChunkHead.alignedSize = 0;
    m_memoryChunkHead.next = &m_memoryChunkHead;
    m_memoryChunkHead.prev = &m_memoryChunkHead;

    //m_allMemory = static_cast<zp_byte*>( zp_malloc( m_allocatedMemorySize + sizeof( zp_uint ) ) );
    //*( reinterpret_cast<zp_uint*>( m_allMemory + m_allocatedMemorySize ) ) = ZP_BLOCK_ALLOCATOR_END_CANARY;
    //
    //zp_size_t headerAlignmentDistance = ZP_BLOCK_ALLOCATOR_INCREMENT_SIZE - ( (zp_size_t)m_allMemory & ZP_BLOCK_ALLOCATOR_INCREMENT_MASK );
    //m_alignedMemory = m_allMemory + headerAlignmentDistance;
    //
    //zpMemoryBlock* block = reinterpret_cast<zpMemoryBlock*>( m_alignedMemory );
    //block->size = 0;
    //block->alignedSize = m_allocatedMemorySize - headerAlignmentDistance;
    //block->next = block;
    //block->prev = block;
    //
    //m_memoryBlockHead = block;
}

zpBlockAllocator::~zpBlockAllocator()
{
    ZP_ASSERT_WARN( m_totalMemoryUsed == 0, "Possible memory leak of %d", m_totalMemoryUsed );
    ZP_ASSERT_WARN( m_numAllocs == 0, "Missing %d allocs->frees", m_numAllocs );

    zpMemoryChunk* chunk = m_memoryChunkHead.next;
    while( chunk != &m_memoryChunkHead )
    {
        zpMemoryChunk* next = chunk->next;

        _freeChunk( chunk );

        chunk = next;
    }
    //ZP_ASSERT( *( reinterpret_cast<zp_uint*>( m_allMemory + m_allocatedMemorySize ) ) == ZP_BLOCK_ALLOCATOR_END_CANARY, "Buffer overrun!" );
    //
    //ZP_ASSERT_WARN( m_memUsed == 0, "Possible memory leak of %d", m_memUsed );
    //ZP_ASSERT_WARN( m_numAllocs == 0, "Missing %d allocs->frees", m_numAllocs );
    //
    //zp_free( m_allMemory );
    //m_allMemory = ZP_NULL;
    //m_alignedMemory = ZP_NULL;

    m_totalMemoryUsed = 0;
    m_totalMemory = 0;
}

void* zpBlockAllocator::allocate( zp_size_t size )
{
    ++m_numAllocs;
    m_totalMemoryUsed += size;

    zp_size_t alignedSize = ZP_BLOCK_ALLOCATOR_ALIGN_SIZE( size + sizeof( zpMemoryBlock ) );

    void* ptr = ZP_NULL;

    zpMemoryChunk* chunk = m_memoryChunkHead.next;

    // find a chunk that has enough space
    while( chunk->alignedSize < alignedSize )
    {
        // if the chunk is the head, create a new chunk
        if( chunk == &m_memoryChunkHead )
        {
            zp_size_t chunkSize = alignedSize < m_minChunkSize ? m_minChunkSize : alignedSize;
            zpMemoryChunk* newChunck = _createChunk( chunkSize );

            newChunck->next = chunk->next;
            newChunck->prev = chunk;

            chunk->next->prev = newChunck;
            chunk->next = newChunck;

            chunk = newChunck;
            break;
        }
        // otherwise, move to next chuck
        else
        {
            chunk = chunk->next;
        }
    }

    zpMemoryBlock* memoryBlockHead = reinterpret_cast<zpMemoryBlock*>( chunk + 1 );

    zpMemoryBlock* block = memoryBlockHead->prev;
    for( ; ; )
    {
        // if the block is not used
        if( block->size == 0 )
        {
            // if alignment size is the same, use block directly
            if( block->alignedSize == alignedSize )
            {
                block->size = size;

                ptr = static_cast<void*>( block + 1 );
                break;
            }
            // otherwise, split block if size is larger
            else if( block->alignedSize > alignedSize )
            {
                // remove aligned size from block
                block->alignedSize -= alignedSize;

                // split
                zp_byte* b = ( reinterpret_cast<zp_byte*>( block ) ) + block->alignedSize;
                zpMemoryBlock* newBlock = reinterpret_cast<zpMemoryBlock*>( b );
                newBlock->size = size;
                newBlock->alignedSize = alignedSize;

                _insertAfter( block, newBlock );

                ptr = static_cast<void*>( newBlock + 1 );
                break;
            }
        }
        // if we've reached the head, break
        else if( block == memoryBlockHead )
        {
            break;
        }

        block = block->prev;
    }

    ZP_ASSERT( ptr, "Failed to allocate memory!" );
    return ptr;
}

void zpBlockAllocator::free( void* ptr )
{
    --m_numAllocs;

    zpMemoryBlock* block = static_cast< zpMemoryBlock* >( ptr ) - 1;

    m_totalMemoryUsed -= block->size;

    block->size = 0;

    // attempt merge next if block and next are contiguous
    zpMemoryBlock* nextBlock = reinterpret_cast<zpMemoryBlock*>( reinterpret_cast<zp_byte*>( block ) + block->alignedSize );
    if( block->next->size == 0 && nextBlock == block->next )
    {
        block->alignedSize += block->next->alignedSize;
        _removeBlock( block->next );
    }

    // attempt merge prev if prev and block are contiguous
    zpMemoryBlock* prevBlock = reinterpret_cast<zpMemoryBlock*>( reinterpret_cast<zp_byte*>( block->prev ) + block->prev->alignedSize );
    if( block->prev->size == 0 && prevBlock == block )
    {
        block->prev->alignedSize += block->alignedSize;
        _removeBlock( block );
    }
}

zp_size_t zpBlockAllocator::getMemoryUsed() const
{
    return m_totalMemoryUsed;
}

zp_size_t zpBlockAllocator::getTotalMemory() const
{
    return m_totalMemory;
}
