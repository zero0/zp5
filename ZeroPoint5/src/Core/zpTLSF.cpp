#include "Core/zpCore.h"

enum zp_tlsf_config
{
#if ZP_64BIT
    ALIGN_SIZE_LOG2 = 3,
#else
    ALIGN_SIZE_LOG2 = 2,
#endif

    SL_INDEX_COUNT_LOG2 = 5,
    SL_INDEX_COUNT = ( 1 << SL_INDEX_COUNT_LOG2 ),

    ALIGN_SIZE = ( 1 << ALIGN_SIZE_LOG2 ),

#if ZP_64BIT
    FL_INDEX_MAX = 32,
#else
    FL_INDEX_MAX = 30,
#endif

    FL_INDEX_SHIFT = ( SL_INDEX_COUNT_LOG2 + ALIGN_SIZE_LOG2 ),
    FL_INDEX_COUNT = ( FL_INDEX_MAX - FL_INDEX_SHIFT + 1 ),

    SMALL_BLOCK_SIZE = ( 1 << FL_INDEX_SHIFT ),
};

typedef struct zp_tlsf_block_header_t
{
    zp_tlsf_block_header_t* prev_block;

    zp_size_t size;

    zp_tlsf_block_header_t* next_free;
    zp_tlsf_block_header_t* prev_free;
} zp_tlsf_block_header_t;

typedef struct zp_tlsf_control_t
{
    zp_tlsf_block_header_t block_null;

    zp_uint fl_bitmap;
    zp_uint sl_bitmap[FL_INDEX_COUNT];

    zp_tlsf_block_header_t* blocks[FL_INDEX_COUNT][SL_INDEX_COUNT];
} zp_tlsf_control_t;

enum zp_tlsf_block_config : zp_size_t
{
    BLOCK_HEADER_FREE_BIT = ( 1 << 0 ),
    BLOCK_HEADER_PREV_FREE_BIT = ( 1 << 1 ),

    BLOCK_HEADER_SET_SIZE_MASK = ( BLOCK_HEADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT ),
    BLOCK_HEADER_GET_SIZE_MASK = ~( BLOCK_HEADER_FREE_BIT | BLOCK_HEADER_PREV_FREE_BIT ),

    BLOCK_HEADER_OVERHEAD = sizeof( zp_size_t ),
    BLOCK_START_OFFSET = offsetof( zp_tlsf_block_header_t, size ) + sizeof( zp_size_t ),
    BLOCK_SIZE_MIN = sizeof( zp_tlsf_block_header_t ) - sizeof( zp_tlsf_block_header_t* ),
    BLOCK_SIZE_MAX = ( (zp_size_t)1 << FL_INDEX_MAX ),
}

// static tests
ZP_STATIC_ASSERT( ALIGN_SIZE == SMALL_BLOCK_SIZE / SL_INDEX_COUNT );


#if ZP_WINDOWS

#include <intrin.h>

#if ZP_64BIT
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#else
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#endif

ZP_FORCE_INLINE zp_int zp_ffs( zp_uint word )
{
    unsigned long index;
    return _BitScanForward( &index, word ) ? index : -1;
}

ZP_FORCE_INLINE zp_int zp_fls( zp_uint word )
{
    unsigned long index;
    return _BitScanReverse( &index, word) ? index : -1;
}

ZP_FORCE_INLINE zp_int zp_ffs_sizet( zp_size_t word )
{
    unsigned long index;
#if ZP_64BIT
    return _BitScanForward64( &index, word ) ? index : -1;
#else
    return _BitScanForward( &index, word ) ? index : -1;
#endif
}

ZP_FORCE_INLINE zp_int zp_fls_sizet( zp_size_t word )
{
    unsigned long index;
#if ZP_64BIT
    return _BitScanReverse64( &index, word ) ? index : -1;
#else
    return _BitScanReverse( &index, word) ? index : -1;
#endif
}

#else
#error "Implement ffs and fls for target platform"
#endif

// block functions
ZP_FORCE_INLINE zp_size_t block_get_size( const zp_tlsf_block_header_t* block )
{
    return block->size & BLOCK_HEADER_GET_SIZE_MASK;
}

ZP_FORCE_INLINE void block_set_size( zp_tlsf_block_header_t* block, zp_size_t size )
{
    const zp_size_t oldSize = block->size;
    block->size = size | ( oldSize & BLOCK_HEADER_SET_SIZE_MASK );
}

ZP_FORCE_INLINE zp_bool block_is_last( const zp_tlsf_block_header_t* block )
{
    return block_get_size( block ) == 0;
}

ZP_FORCE_INLINE zp_bool block_is_free( const zp_tlsf_block_header_t* block )
{
    return ( block->size & BLOCK_HEADER_FREE_BIT ) == BLOCK_HEADER_FREE_BIT;
}

ZP_FORCE_INLINE void block_set_free( zp_tlsf_block_header_t* block )
{
    block->size |= BLOCK_HEADER_FREE_BIT;
}

ZP_FORCE_INLINE void block_set_used( zp_tlsf_block_header_t* block )
{
    block->size &= ~BLOCK_HEADER_FREE_BIT;
}

ZP_FORCE_INLINE zp_bool block_is_prev_free( const zp_tlsf_block_header_t* block )
{
    return ( block->size & BLOCK_HEADER_PREV_FREE_BIT ) == BLOCK_HEADER_PREV_FREE_BIT;
}

ZP_FORCE_INLINE void block_set_prev_free( zp_tlsf_block_header_t* block )
{
    block->size |= BLOCK_HEADER_PREV_FREE_BIT;
}

ZP_FORCE_INLINE void block_set_prev_used( zp_tlsf_block_header_t* block )
{
    block->size &= ~BLOCK_HEADER_PREV_FREE_BIT;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_from_ptr( const void* ptr )
{
    return (zp_tlsf_block_header_t*)( (zp_byte*)ptr - BLOCK_START_OFFSET );
}

ZP_FORCE_INLINE void* block_to_ptr( const zp_tlsf_block_header_t* block )
{
    return (void*)( (zp_byte*)block + BLOCK_START_OFFSET );
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* offset_to_block( const void* ptr, zp_ptrdiff_t size )
{
    return (zp_tlsf_block_header_t*)( (zp_ptrdiff_t)ptr + size );
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_prev( const zp_tlsf_block_header_t* block )
{
    ZP_ASSERT( block_is_prev_free( block ), "Previous block must be free" );
    return block->prev_block;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_next( const zp_tlsf_block_header_t* block )
{
    ZP_ASSERT( !block_is_last( block ), "Block is last" );
    zp_tlsf_block_header_t* next = offset_to_block( block_to_ptr( block ), block_get_size( block ) - BLOCK_HEADER_OVERHEAD );
    return next;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_link_next( zp_tlsf_block_header_t* block )
{
    zp_tlsf_block_header_t* next = block_next( block );
    next->prev_block = block;
    return next;
}

ZP_FORCE_INLINE void block_mark_as_free( zp_tlsf_block_header_t* block )
{
    zp_tlsf_block_header_t* next = block_link_next( block );
    block_set_prev_free( next );
    block_set_free( block );
}

ZP_FORCE_INLINE void block_mark_as_used( zp_tlsf_block_header_t* block )
{
    zp_tlsf_block_header_t* next = block_next( block );
    block_set_prev_used( next );
    block_set_used( block );
}

// alignment util
ZP_FORCE_INLINE zp_size_t align_up( zp_size_t x, zp_size_t align )
{
    ZP_ASSERT( ( align & ( align - 1 ) ) == 0, "Must align to a power of two" );
    const zp_size_t r = ( x + ( align - 1 ) ) & ~( align - 1 );
    return r;
}

ZP_FORCE_INLINE zp_size_t align_down( zp_size_t x, zp_size_t align )
{
    ZP_ASSERT( ( align & ( align - 1 ) ) == 0, "Must align to a power of two" );
    const zp_size_t r = x - ( x & ( align - 1 ) );
    return r;
}

ZP_FORCE_INLINE void* align_ptr( const void* ptr, zp_size_t align )
{
    ZP_ASSERT( ( align & ( align - 1 ) ) == 0, "Must align to a power of two" );
    const zp_ptrdiff_t aligned = ( (zp_ptrdiff_t)ptr + ( align - 1 ) ) & ~( align - 1 );
    return (void*)aligned;
}

ZP_FORCE_INLINE zp_size_t adjust_requested_size( zp_size_t size, zp_size_t align )
{
    zp_size_t adjusted = 0;
    if( size && size < BLOCK_SIZE_MAX )
    {
        const zp_size_t aligned = align_up( size, align );
        adjusted = ZP_MAX( aligned, BLOCK_SIZE_MIN );
    }
    return adjusted;
}

// tlsf util
ZP_FORCE_INLINE void mapping_insert( zp_size_t size, zp_int& fl, zp_int& sl )
{
    if( size < SMALL_BLOCK_SIZE )
    {
        fl = 0;
        sl = static_cast<zp_int>( size / ( SMALL_BLOCK_SIZE / SL_INDEX_COUNT ) );
    }
    else
    {
        fl = zp_fls_sizet( size );
        sl = static_cast<zp_int>( size >> ( fl - SL_INDEX_COUNT_LOG2 ) ) ^ SL_INDEX_COUNT;
        fl -= ( FL_INDEX_SHIFT - 1 );
    }
}

ZP_FORCE_INLINE void mapping_search( zp_size_t size, zp_int& fl, zp_int& sl )
{
    if( size >= SMALL_BLOCK_SIZE )
    {
        const zp_size_t round = ( 1 << ( zp_fls_sizet( size ) - SL_INDEX_COUNT_LOG2 ) ) - 1;
        size += round;
    }
    mapping_insert( size, fl, sl );
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* search_suitable_block( const zp_tlsf_control_t* control, zp_int& fl, zp_int& sl )
{
    zp_uint sl_map = control->sl_bitmap[ fl ] & ( ~0U << sl );
    if( !sl_map )
    {
        const zp_uint fl_map = control->fl_bitmap & ( ~0U << ( fl + 1 ) );
        if( !fl_map )
        {
            return ZP_NULL;
        }

        fl = zp_ffs( fl_map );
        sl_map = control->sl_bitmap[ fl ];
        ZP_ASSERT( sl_map, "Second level bitmap is null" );
    }

    sl = zp_ffs( sl_map );

    zp_tlsf_block_header_t* header = control->blocks[fl][sl];
    return header;
}

ZP_FORCE_INLINE void remove_free_block( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, zp_int fl, zp_int sl )
{
    zp_tlsf_block_header_t* prev = block->prev_free;
    zp_tlsf_block_header_t* next = block->next_free;

    ZP_ASSERT( prev, "Prev must be set" );
    ZP_ASSERT( next, "Next must be set" );

    next->prev_free = prev;
    prev->next_free = next;

    // if the block is the head of the free list, set a new head
    if( control->blocks[fl][sl] == block )
    {
        control->blocks[fl][sl] = next;

        // if the new head is null, clear second level bitmap
        if( next == &control->block_null )
        {
            control->sl_bitmap[fl] &= ~( 1 << sl );

            // if second level is empty, clear first
            if( !control->sl_bitmap[fl] )
            {
                control->fl_bitmap &= ~( 1 << fl );
            }
        }
    }
}

ZP_FORCE_INLINE void insert_free_block( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, int fl, int sl )
{
    ZP_ASSERT( block, "Cannot insert null into the free list" );

    zp_tlsf_block_header_t* current = control->blocks[fl][sl];
    ZP_ASSERT( current, "Free list cannot have a null entry" );

    block->next_free = current;
    block->prev_free = &control->block_null;
    current->prev_free = block;

    ZP_ASSERT( block_to_ptr( block ) == align_ptr( block_to_ptr( block ), ALIGN_SIZE ), "Block not aligned properly" );

    // insert block at head of the list and mark bitmaps
    control->blocks[fl][sl] = block;
    control->fl_bitmap |= ( 1 << fl );
    control->sl_bitmap[fl] |= ( 1 << sl );
}

ZP_FORCE_INLINE void block_remove( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block )
{
    zp_int fl;
    zp_int sl;

    const zp_size_t size = block_get_size( block );
    mapping_insert( size, fl, sl );
    remove_free_block( control, block, fl, sl );
}

ZP_FORCE_INLINE void block_insert( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block )
{
    zp_int fl;
    zp_int sl;

    const zp_size_t size = block_get_size( block );
    mapping_insert( size, fl, sl );
    insert_free_block( control, block, fl, sl );
}

ZP_FORCE_INLINE zp_bool block_can_split( const zp_tlsf_block_header_t* block, zp_size_t size )
{
    const zp_size_t blockSize = block_get_size( block );
    return blockSize >= sizeof( zp_tlsf_block_header_t ) + size;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_split( zp_tlsf_block_header_t* block, zp_size_t size )
{
    zp_tlsf_block_header_t* remaining = offset_to_block( block_to_ptr( block ), size - BLOCK_HEADER_OVERHEAD );

    const zp_size_t remainingSize = block_get_size( block ) - ( size + BLOCK_HEADER_OVERHEAD );

    ZP_ASSERT( block_to_ptr( remaining ) == align_ptr( block_to_ptr( remaining ), ALIGN_SIZE ), "Remaining block not aligned properly" );
    ZP_ASSERT( block_get_size( block ) == remainingSize + size + BLOCK_HEADER_OVERHEAD, "Block split size incorrect" );

    block_set_size( remaining, remainingSize );
    ZP_ASSERT( block_get_size( remaining ) >= BLOCK_SIZE_MIN, "Block split with invalid size" );
    
    block_set_size( block, size );
    block_mark_as_free( remaining );

    return remaining;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_absorb( zp_tlsf_block_header_t* prev, const zp_tlsf_block_header_t* block )
{
    ZP_ASSERT( !block_is_last( prev ), "Previous block cannot be last" );

    prev->size += block_get_size( block ) + BLOCK_HEADER_OVERHEAD;
    block_link_next( prev );

    return prev;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_merge_prev( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block )
{
    if( block_is_prev_free( block ) )
    {
        zp_tlsf_block_header_t* prev = block->prev_block;
        ZP_ASSERT( prev, "Previous block cannot be null" );
        ZP_ASSERT( block_is_free( prev ), "Previous block is not free" );

        block_remove( control, prev );
        block = block_absorb( prev, block );
    }

    return block;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_merge_next( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block )
{
    zp_tlsf_block_header_t* next = block_next( block );
    ZP_ASSERT( next, "Next block cannot be null" );

    if( block_is_free( next ) )
    {
        ZP_ASSERT( block_is_free( block ), "Previous block is not free" );

        block_remove( control, next );
        block = block_absorb( block, next );
    }

    return block;
}

ZP_FORCE_INLINE void block_trim_free( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, zp_size_t size )
{
    ZP_ASSERT( block_is_free( block ), "Block must be free" );
    if( block_can_split( block, size ) )
    {
        zp_tlsf_block_header_t* remaining = block_split( block, size );
        block_link_next( block );
        block_set_prev_free( remaining );
        block_insert( control, remaining );
    }
}

ZP_FORCE_INLINE void block_trim_used( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, zp_size_t size )
{
    ZP_ASSERT( !block_is_free( block ), "Block must be used" );
    if( block_can_split( block, size ) )
    {
        zp_tlsf_block_header_t* remaining = block_split( block, size );
        block_set_prev_used( remaining );

        remaining = block_merge_next( control, remaining );
        block_insert( control, remaining );
    }
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_trim_free_leading( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, zp_size_t size )
{
    zp_tlsf_block_header_t* remaining = block;

    if( block_can_split( block, size ) )
    {
        remaining = block_split( block, size - BLOCK_HEADER_OVERHEAD );
        block_set_prev_free( remaining );

        block_link_next( block );
        block_insert( control, block );
    }

    return remaining;
}

ZP_FORCE_INLINE zp_tlsf_block_header_t* block_locate_free( zp_tlsf_control_t* control, zp_size_t size )
{
    zp_int fl = 0;
    zp_int sl = 0;
    zp_tlsf_block_header_t* block = ZP_NULL;

    if( size )
    {
        mapping_search( size, fl, sl );
        block = search_suitable_block( control, fl, sl );
    }

    if( block )
    {
        ZP_ASSERT( block_get_size( block ) >= size, "Block incorrect size" );
        remove_free_block( control, block, fl, sl );
    }

    return block;
}

ZP_FORCE_INLINE void* block_prepare_unsed( zp_tlsf_control_t* control, zp_tlsf_block_header_t* block, zp_size_t size )
{
    void* ptr = ZP_NULL;
    if( block )
    {
        ZP_ASSERT( size, "Size must be non-zero" );
        block_trim_free( control, block, size );
        block_mark_as_used( block );
        ptr = block_to_ptr( block );
    }
    return ptr;
}

ZP_FORCE_INLINE void control_reset( zp_tlsf_control_t* control )
{
    zp_tlsf_block_header_t* nullBlock = &control->block_null;
    control->block_null.next_free = nullBlock;
    control->block_null.prev_free = nullBlock;

    control->fl_bitmap = 0;

    zp_int f;
    zp_int s;
    for( f = 0; f < FL_INDEX_COUNT; ++f )
    {
        control->sl_bitmap[f] = 0;
        for( s = 0; s < SL_INDEX_COUNT; ++s )
        {
            control->blocks[f][s] = nullBlock;
        }
    }
}

// interface methods
zp_size_t zp_tlsf_size()
{
    return sizeof( zp_tlsf_control_t );
}

zp_tlsf_pool_t zp_tlsf_get_pool( zp_tlsf_t tlsf )
{
	return static_cast<zp_tlsf_pool_t>( (zp_byte*)tlsf + sizeof( zp_tlsf_control_t ) );
}

zp_tlsf_t zp_tlsf_create( void* memory )
{
    if( ( (zp_ptrdiff_t)memory % ALIGN_SIZE ) != 0 )
    {
        zp_printf( "Memory must be aligned to %u bytes", ALIGN_SIZE );
        return ZP_NULL;
    }

    zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( memory );
    control_reset( control );

    return static_cast<zp_tlsf_t>( memory );
}

zp_tlsf_t zp_tlsf_create_with_pool( void* memory, zp_size_t size )
{
    zp_tlsf_t tlsf = zp_tlsf_create( memory );
    zp_tlsf_add_pool( tlsf, (zp_byte*)memory + sizeof( zp_tlsf_control_t ), size - sizeof( zp_tlsf_control_t ) );
    return tlsf;
}

void zp_tlsf_destroy( zp_tlsf_t tlsf )
{
    zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( tlsf );
    ZP_ASSERT( control->fl_bitmap == 0, "TLSF not cleared" );
}

zp_tlsf_pool_t zp_tlsf_add_pool( zp_tlsf_t tlsf, void* memory, zp_size_t size )
{
    if( ( (zp_ptrdiff_t)memory % ALIGN_SIZE ) != 0 )
    {
        zp_printf( "Memory must be aligned to %u bytes", ALIGN_SIZE );
        return ZP_NULL;
    }

    const zp_size_t poolOverhead = 2 * BLOCK_HEADER_OVERHEAD;
    const zp_size_t aligned = align_down( size - poolOverhead, ALIGN_SIZE );
    if( aligned < BLOCK_SIZE_MIN || aligned > BLOCK_SIZE_MAX )
    {
#if ZP_64BIT
        zp_printf( "Memory size must be between %llx and %llx bytes", ( poolOverhead + BLOCK_SIZE_MIN ), ( poolOverhead + BLOCK_SIZE_MAX ) );
#else
        zp_printf( "Memory size must be between %u and %u bytes", ( poolOverhead + BLOCK_SIZE_MIN ), ( poolOverhead + BLOCK_SIZE_MAX ) );
#endif
        return ZP_NULL;
    }

    // add main free block
    zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( tlsf );

    zp_tlsf_block_header_t* block = offset_to_block( memory, -(zp_ptrdiff_t)BLOCK_HEADER_OVERHEAD );
    block_set_size( block, aligned );
    block_set_free( block );
    block_set_prev_used( block );
    block_insert( control, block );

    // create zero-sized sentinel block
    zp_tlsf_block_header_t* next = block_link_next( block );
    block_set_size( next, 0 );
    block_set_used( next );
    block_set_prev_free( next );

    return memory;
}

void zp_tlsf_remove_pool( zp_tlsf_t tlsf, zp_tlsf_pool_t pool )
{
    zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( tlsf );
    zp_tlsf_block_header_t* block = offset_to_block( pool, -(zp_ptrdiff_t)BLOCK_HEADER_OVERHEAD );

    ZP_ASSERT( block_is_free( block ), "Block should be free" );
    ZP_ASSERT( !block_is_free( block_next( block ) ), "Next block should not be free" );
    ZP_ASSERT( block_get_size( block_next( block ) ) == 0, "Next block size should be zero" );

    zp_int fl;
    zp_int sl;
    mapping_insert( block_get_size( block ), fl, sl );
    remove_free_block( control, block, fl, sl );
}

void* zp_tlsf_malloc( zp_tlsf_t tlsf, zp_size_t size )
{
    zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( tlsf );
    const zp_size_t adjusted = adjust_requested_size( size, ALIGN_SIZE );
    zp_tlsf_block_header_t* block = block_locate_free( control, adjusted );
    void* ptr = block_prepare_unsed( control, block, adjusted );
    return ptr;
}

void zp_tlsf_free( zp_tlsf_t tlsf, const void* ptr )
{
    if( ptr )
    {
        zp_tlsf_control_t* control = static_cast<zp_tlsf_control_t*>( tlsf );
        zp_tlsf_block_header_t* block = block_from_ptr( ptr );
        block_mark_as_free( block );
        block = block_merge_prev( control, block );
        block = block_merge_next( control, block );
        block_insert( control, block );
    }
}

zp_size_t zp_tlsf_block_size( const void* ptr )
{
    zp_size_t size = 0;
    if( ptr )
    {
        const zp_tlsf_block_header_t* block = block_from_ptr( ptr );
        size = block_get_size( block );
    }
    return size;
}

zp_size_t zp_tlsf_pool_size( zp_tlsf_pool_t pool )
{
    zp_size_t size = 0;
    if( pool )
    {
        const zp_tlsf_block_header_t* block = offset_to_block( pool, -(zp_ptrdiff_t)BLOCK_HEADER_OVERHEAD );
        size = block_get_size( block );
    }
    return size;
}
