#include "Core/zpCore.h"
#include "Engine/zpEngine.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

zpBlockAllocator g_globalAllocator( ZP_MEMORY_MB( 10 ) );

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif // !ZP_WINDOWS
{
    int r;// = zpMain();

    zpBaseApplication app;
    app.setHandle( hInstance );
    app.processCommandLine( lpCmdLine );

    zpString bb( "world" );
    zpString aa( "hello" );
    aa.assign( "tes" );
    aa.assign( "more string" );
    aa = bb;

    app.initialize();
    app.run();
    r = app.shutdown();

    return r;
}
