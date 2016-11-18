#include "Core/zpCore.h"

int main( const char** args )
{
    zpStackAllocator< 1000 > mem;

    void* a = mem.allocate( 11 );
    void* b = mem.allocate( 250 );
    void* c = mem.allocate( 532 );

    mem.free( b );
    mem.free( a );
    mem.free( c );

    return 0;
}
