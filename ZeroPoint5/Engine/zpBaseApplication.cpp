#include "zpEngine.h"

#define ZP_WINDOW_CLASS_NAME "zpWindowClass"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

LRESULT CALLBACK _WinProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    switch( uMessage )
    {
        case WM_CLOSE:
            PostQuitMessage( 0 );
            break;

        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
            lpMMI->ptMinTrackSize = { 300, 300 };
        }
            break;

        case WM_KILLFOCUS:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->setApplicationFocus( false );
        }
            break;

        case WM_SETFOCUS:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->setApplicationFocus( true );
        }
            break;
    }

    return DefWindowProc( hWnd, uMessage, wParam, lParam );
}

ZP_FORCE_INLINE void _AdjustWindowSize( zpRecti& windowRect, DWORD windowStyle )
{
    RECT rc = { 0, 0, windowRect.width, windowRect.height };
    AdjustWindowRectEx( &rc, windowStyle, false, 0 );
    windowRect.width = rc.right - rc.left;
    windowRect.height = rc.bottom - rc.top;
}

#endif // ZP_WINDOWS

zpBaseApplication::zpBaseApplication()
    : m_hWnd( ZP_NULL )
    , m_hInstance( ZP_NULL )
    , m_frameCount( 0 )
    , m_targetFps( 60 )
    , m_targetFixedFps( 30 )
    , m_exitCode( ZP_APPLICATION_EXIT_NORMAL )
    , m_isRunning( false )
    , m_isPaused( false )
    , m_isFocused( false )
    , m_shouldRestart( false )
    , m_shouldGarbageCollect( false )
    , m_shouldReloadAllResources( false )
    , m_shouldPauseInBackground( false )
{

}

zpBaseApplication::~zpBaseApplication()
{
    m_hInstance = ZP_NULL;
    m_hWnd = ZP_NULL;
}

void zpBaseApplication::processCommandLine( const zp_char* commandline )
{

}

void zpBaseApplication::setHandle( zp_handle instance )
{
    m_hInstance = instance;
}

void zpBaseApplication::initialize()
{
    onPreInitialize();

    createWindow();

    onPostInitialize();
}

void zpBaseApplication::setup()
{
    onPreSetup();

    m_renderingEngine.setup( m_hWnd );

    onPostSetup();
}

void zpBaseApplication::run()
{
    do
    {
        m_isRunning = true;
        m_shouldRestart = false;
 
        setup();

        while( m_isRunning && processMessages() )
        {
            processFrame();
        }

        teardown();
    } while( m_shouldRestart );
}

void zpBaseApplication::teardown()
{
    onPreTeardown();

    m_objectManager.garbageCollect();

    m_renderingEngine.teardown();

    onPostTeardown();
}

zpApplicationExitCode zpBaseApplication::shutdown()
{
    onPreShutdown();

    destroyWindow();

    onPostShutdown();

    return m_exitCode;
}

void zpBaseApplication::setPaused( zp_bool paused )
{
    m_isPaused = paused;
}

zp_bool zpBaseApplication::isPaused() const
{
    return m_isPaused;
}

void zpBaseApplication::restart()
{
    m_shouldRestart = true;
    m_isRunning = false;
}

void zpBaseApplication::exit( zpApplicationExitCode exitCode )
{
    m_isRunning = false;
    m_exitCode = exitCode;
}

zp_bool zpBaseApplication::shouldRestart() const
{
    return m_shouldRestart;
}

zp_bool zpBaseApplication::isRunning() const
{
    return m_isRunning;
}

void zpBaseApplication::garbageCollect()
{
    m_shouldGarbageCollect = true;
}

void zpBaseApplication::reloadAllResouces()
{
    m_shouldReloadAllResources = true;
}

void zpBaseApplication::setApplicationFocus( zp_bool focus )
{
    m_isFocused = focus;
}

void zpBaseApplication::runGarbageCollection()
{
    m_objectManager.garbageCollect();

    onGarbageCollection();
}

void zpBaseApplication::runReloadAllResources()
{
    onReloadAllResources();
}

void zpBaseApplication::createWindow()
{
#ifdef ZP_WINDOWS
    WNDCLASSEX wc;
    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = _WinProc;
    wc.hInstance = static_cast<HINSTANCE>( m_hInstance );
    wc.hbrBackground = static_cast<HBRUSH>( GetStockObject( LTGRAY_BRUSH ) );
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ZP_WINDOW_CLASS_NAME;

    ATOM reg = RegisterClassEx( &wc );
    ZP_ASSERT( reg, "Unable to register window class" );

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME;
    DWORD exStyle = 0;
#ifdef ZP_DEBUG
    exStyle |= WS_EX_ACCEPTFILES;
#endif

    const zp_char* title = "ZeroPoint " ZP_VERSION;
    zpRecti windowRect = { CW_USEDEFAULT, CW_USEDEFAULT, 960, 640 };

    _AdjustWindowSize( windowRect, style );

    HWND hWnd = CreateWindowEx(
        exStyle,
        ZP_WINDOW_CLASS_NAME,
        title,
        style,
        windowRect.x,
        windowRect.y,
        windowRect.width,
        windowRect.height,
        NULL,
        NULL,
        static_cast<HINSTANCE>( m_hInstance ),
        NULL
    );

    ZP_ASSERT( hWnd, "Unable to create window" );

    SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );

    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow( hWnd );

    m_hWnd = hWnd;
#else
#error Not implmemented on other platforms
#endif
}

void zpBaseApplication::destroyWindow()
{
#ifdef ZP_WINDOWS
    DestroyWindow( static_cast<HWND>( m_hWnd ) );
    UnregisterClass( ZP_WINDOW_CLASS_NAME, static_cast<HINSTANCE>( m_hInstance ) );
#endif

    m_hWnd = ZP_NULL;
}

zp_bool zpBaseApplication::processMessages()
{
    zp_bool messageOk = true;

#ifdef ZP_WINDOWS
    MSG message;
    if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
    {
        if( message.message == WM_QUIT )
        {
            messageOk = false;
        }

        TranslateMessage( &message );
        DispatchMessage( &message );
    }
#endif // ZP_WINDOWS

    return messageOk;
}

void zpBaseApplication::processFrame()
{
    zp_bool paused = m_isPaused || ( m_shouldPauseInBackground && !m_isFocused );

    m_time.tick();
    zp_long startTime = m_time.getTime();
    zp_float dt = paused ? 0.f : m_time.getDeltaSeconds();
    zp_float rt = m_time.getActualDeltaSeconds();

    if( m_shouldGarbageCollect )
    {
        m_shouldGarbageCollect = false;

        runGarbageCollection();
    }

    if( m_shouldReloadAllResources )
    {
        m_shouldReloadAllResources = false;

        runReloadAllResources();
    }

    onUpdate( dt, rt );

    onLateUpdate( dt, rt );

    m_renderingEngine.present();

    ++m_frameCount;

    zp_long endTime = m_time.getTime();

    zp_float spf = 1.f / static_cast<zp_float>( m_targetFps );
    zp_long diff = ( endTime - startTime ) * 1000LL;
    zp_float d = diff * m_time.getSecondsPerTick();
    zp_float sleepTime = ( 1000.f * spf ) - d;
    while( sleepTime < 0.f )
    {
        sleepTime += spf;
    }

    zp_sleep( static_cast<zp_int>( sleepTime ) );
}
