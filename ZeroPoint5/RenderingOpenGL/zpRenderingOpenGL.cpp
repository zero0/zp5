#include "Core\zpCore.h"
#include "zpRenderingOpenGL.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

#define GLEW_STATIC
#include <GL\glew.h>

void SetupRenderingOpenGL( zp_handle hWindow, zp_handle& hDC, zp_handle& hContext )
{
#ifdef ZP_WINDOWS
    BOOL ok;
    HWND wnd = static_cast<HWND>( hWindow );
    HDC dc = GetDC( wnd );

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

    zp_int pixelFormatIndex = ChoosePixelFormat( dc, &pfd );
    ZP_ASSERT( pixelFormatIndex != 0, "" );

    ok = SetPixelFormat( dc, pixelFormatIndex, &pfd );
    ZP_ASSERT( ok, "" );

    HGLRC context = wglCreateContext( dc );
    ok = wglMakeCurrent( dc, context );
    ZP_ASSERT( ok, "" );

    hDC = dc;
    hContext = context;
#endif

    glewExperimental = true;
    GLenum r = glewInit();
    ZP_ASSERT( r == GLEW_OK, "" );

    zp_printfln( "Using GLEW: %s", glewGetString( GLEW_VERSION ) );
    zp_printfln( "Using GL:   %s", glGetString( GL_VERSION ) );
}

void TeardownRenderingOpenGL( zp_handle hContext )
{
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( ZP_NULL, ZP_NULL );
    wglDeleteContext( context );
}

void ProcessRenderingCommandOpenGL( zpRenderingCommand* cmd )
{
    switch( cmd->type )
    {
        case ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT:
            glScissor( cmd->scissorRect.x,
                       cmd->scissorRect.y,
                       cmd->scissorRect.width,
                       cmd->scissorRect.height );
            break;

        case ZP_RENDERING_COMMNAD_SET_VIEWPORT:
            glViewport( cmd->viewport.topX,
                        cmd->viewport.height - cmd->viewport.topY,
                        cmd->viewport.width,
                        cmd->viewport.height );
            break;

        case ZP_RENDERING_COMMNAD_CLEAR:
            glClearColor( cmd->clearColor.r,
                          cmd->clearColor.g,
                          cmd->clearColor.b,
                          cmd->clearColor.a );
            glClearDepthf( cmd->clearDepth );
            glClearStencil( cmd->clearStencil );
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
            break;

        case ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE:
            break;

        case ZP_RENDERING_COMMNAD_DRAW_BUFFERED:
            break;

        case ZP_RENDERING_COMMNAD_DRAW_INSTANCED:
            break;

        default:
            break;
    }
}

void PresentOpenGL( zp_handle hDC, zp_handle hContext )
{
    HDC dc = static_cast<HDC>( hDC );
    HGLRC context = static_cast<HGLRC>( hContext );

    wglMakeCurrent( dc, context );

    glViewport( 0, 0, 960, 640 );
    glClearColor( 0.2058f, 0.3066f, 0.4877f, 1.0f );
    glClearDepthf( 1.0f );
    glClearStencil( 0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    glLoadIdentity();

    glBegin( GL_TRIANGLES );                      // Drawing Using Triangles
    glColor4f( 1, 0, 0, 1 );
    glVertex3f( 0.0f, 1.0f, 0.0f );              // Top
    glColor4f( 0, 1, 0, 1 );
    glVertex3f( -1.0f, -1.0f, 0.0f );              // Bottom Left
    glColor4f( 0, 0, 1, 1 );
    glVertex3f( 1.0f, -1.0f, 0.0f );              // Bottom Right
    glEnd();                            // Finished Drawing The Triangle

    glFlush();

    SwapBuffers( dc );
}
