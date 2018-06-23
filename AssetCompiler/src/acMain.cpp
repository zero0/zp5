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

static zpMemoryAllocator< zpHeapMemoryStorage< ZP_MEMORY_MB( 10 ) >, zpTLFSMemoryPolicy > s_globalAllocator;

#ifdef ZP_WINDOWS
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif // !ZP_WINDOWS
{
    g_globalAllocator = &s_globalAllocator;

    int r = 0;
    {
        acApplication app;

        r = app.processCommandLine( lpCmdLine );
        if( r == 0 )
        {
            r = app.run();
        }
    }

    return r;
}
