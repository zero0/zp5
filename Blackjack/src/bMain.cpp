#include "Core/zpCore.h"
#include "Engine/zpEngine.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#endif // !ZP_WINDOWS

#ifdef ZP_USE_PROFILER
zpProfiler g_profiler;
#endif

zpIMemoryAllocator* g_globalAllocator;

#if ZP_DEBUG
zpIMemoryAllocator* g_globalDebugAllocator;
#endif


#ifdef ZP_WINDOWS
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif // !ZP_WINDOWS
{
#if ZP_DEBUG
    zpStackTrace::Initialize();

    zpMemoryAllocator< zpHeapMemoryStorage< ZP_MEMORY_MB( 8 ) >, zpTLFSMemoryPolicy > globalDebugAllocator;
    globalDebugAllocator.setup();
    g_globalDebugAllocator = &globalDebugAllocator;
#endif

    zpMemoryAllocator< zpHeapMemoryStorage< ZP_MEMORY_MB( 32 ) >, zpTLFSMemoryPolicy > globalAllocator;
    globalAllocator.setup();
    g_globalAllocator = &globalAllocator;

    {
        zpMap<zp_int, zp_float> mm;
        mm.set( 0, 0 );
        mm.set( 1, 1 );
        mm.set( 2, 2 );
        mm.set( 3, 3 );
        mm.remove( 2 );
        mm.set( 4, 4 );
        mm.set( 5, 5 );
        mm.set( 6, 6 );
        mm.remove( 0 );

        zpMap<zp_int, zp_float>::iterator b = mm.begin();
        zpMap<zp_int, zp_float>::iterator e = mm.end();
        for( ; b != e; ++b )
        {
            b.value() += 0.5f;
            zp_printfln( "%d -> %f", b.key(), b.value() );
        }
    }

    int r = 0;// = zpMain();
    {
        zpBaseApplication app;
        app.setHandle( hInstance );
        app.processCommandLine( lpCmdLine );

        app.initialize();
        app.run();
        r = app.shutdown();
    }

    globalAllocator.teardown();
    g_globalAllocator = ZP_NULL;

#if ZP_DEBUG
    globalDebugAllocator.teardown();
    g_globalDebugAllocator = ZP_NULL;

    zpStackTrace::Shutdown();
#endif

    return r;
}
