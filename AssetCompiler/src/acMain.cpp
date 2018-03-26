#include "Core/zpCore.h"
#include "acMain.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // !ZP_WINDOWS

#ifdef ZP_USE_PROFILER
zpProfiler g_profiler;
#endif
zpBlockAllocator g_globalAllocator( ZP_MEMORY_MB( 10 ) );

#ifdef ZP_WINDOWS
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif // !ZP_WINDOWS
{
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
