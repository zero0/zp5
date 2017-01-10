#include "Core\zpCore.h"
#include "zpRenderingOpenGL.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

#include <gl\GL.h>

zpRenderingEngineOpenGL::zpRenderingEngineOpenGL()
{

}

zpRenderingEngineOpenGL::~zpRenderingEngineOpenGL()
{

}

void zpRenderingEngineOpenGL::setup( zp_handle hWindow )
{
    BOOL ok;
    HWND hWnd = static_cast<HWND>( hWindow );
    HDC hDC = GetDC( hWnd );

    PIXELFORMATDESCRIPTOR pfd;
    zp_memset( &pfd, 0, sizeof( PIXELFORMATDESCRIPTOR ) );

    pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    zp_int pixelFormatIndex = ChoosePixelFormat( hDC, &pfd );
    ZP_ASSERT( pixelFormatIndex != 0, "" );

    ok = SetPixelFormat( hDC, pixelFormatIndex, &pfd );
    ZP_ASSERT( ok, "" );

    HGLRC hContext = wglCreateContext( hDC );
    ok = wglMakeCurrent( hDC, hContext );
    ZP_ASSERT( ok, "" );

    m_hContext = hContext;
    m_hWindow = hWindow;
    m_hDC = hDC;
}
void zpRenderingEngineOpenGL::teardown()
{
    wglMakeCurrent( ZP_NULL, ZP_NULL );
    wglDeleteContext( static_cast<HGLRC>( m_hContext ) );

    m_hContext = ZP_NULL;
    m_hWindow = ZP_NULL;
    m_hDC = ZP_NULL;
}

void zpRenderingEngineOpenGL::setWindowSize( const zpVector2i& size )
{
    
}

void zpRenderingEngineOpenGL::present()
{
    HDC hDC = static_cast<HDC>( m_hDC );
    HGLRC hContext = static_cast<HGLRC>( m_hContext );

    wglMakeCurrent( hDC, hContext );

    glClearColor( 0.2058f, 0.3066f, 0.4877f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    glLoadIdentity();

    glBegin( GL_TRIANGLES );                      // Drawing Using Triangles
    glColor4f( 0, 0, 0, 1 );
    glVertex3f( 0.0f, 1.0f, 0.0f );              // Top
    glColor4f( 0, 0, 0, 1 );
    glVertex3f( -1.0f, -1.0f, 0.0f );              // Bottom Left
    glColor4f( 0, 0, 0, 1 );
    glVertex3f( 1.0f, -1.0f, 0.0f );              // Bottom Right
    glEnd();                            // Finished Drawing The Triangle

    glFlush();

    SwapBuffers( hDC );
}
