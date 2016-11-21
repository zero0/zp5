#include "Core/zpCore.h"
#include "Engine/zpEngine.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
#endif // !ZP_WINDOWS
{
    int r = zpMain( );
    return r;
}
