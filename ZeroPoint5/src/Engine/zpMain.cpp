#include "Engine/zpEngine.h"

zp_int zpMain()
{
    zpStackAllocator< 1000 > mem;

    void* a = mem.allocate( 11 );
    void* b = mem.allocate( 250 );
    void* c = mem.allocate( 532 );

    mem.free( c );
    mem.free( b );
    mem.free( a );

    zpVector4f v1 = zpMath::Vector4( 0, 1, 2, 1 );
    zpVector4f v2 = zpMath::Vector4( 2, 3, 5, 1 );

    zpScalar dot = zpMath::Vector4Dot3( v1, v2 );

    zpBlockAllocator block( 10000 );
    void* d = block.allocate( 1234 );
    void* e = block.allocate( 5678 );

    block.free( d );
    block.free( e );

    return 0;
}
