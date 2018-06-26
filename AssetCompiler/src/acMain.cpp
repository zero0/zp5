#include "acMain.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

#ifdef ZP_USE_PROFILER
zpProfiler g_profiler;
#endif

//zpBlockAllocator g_globalAllocator( ZP_MEMORY_MB( 10 ) );
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

    int r = 0;
    {
        acApplication app;

        r = app.processCommandLine( lpCmdLine );
        if( r == 0 )
        {
            r = app.run();
        }
    }

    globalAllocator.teardown();

#if ZP_DEBUG
    globalDebugAllocator.teardown();

    zpStackTrace::Shutdown();
#endif

    return r;
}
