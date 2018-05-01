#include "Core/zpCore.h"
#include "Engine/zpEngine.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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
    zpStackTrace::Initialize();

    zpMemoryAllocator< zpHeapMemoryStorage< ZP_MEMORY_MB( 32 ) >, zpTLFSMemoryPolicy > globalAllocator;

#if ZP_DEBUG
    zpMemoryAllocator< zpHeapMemoryStorage< ZP_MEMORY_MB( 8 ) >, zpTLFSMemoryPolicy > globalDebugAllocator;
    globalDebugAllocator.setup();
    g_globalDebugAllocator = &globalDebugAllocator;
#endif

    globalAllocator.setup();
    g_globalAllocator = &globalAllocator;

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
#if ZP_DEBUG
    globalDebugAllocator.teardown();
#endif

    zpStackTrace::Shutdown();

    return r;
}
