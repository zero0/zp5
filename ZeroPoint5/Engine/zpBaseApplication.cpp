#include "zpEngine.h"

enum zpBaseApplicationFlags : zp_ulong
{
    ZP_BASE_APPLICATION_FLAG_IS_RUNNING =       1ULL << 0,
    ZP_BASE_APPLICATION_FLAG_IS_PAUSED =        1ULL << 1,
    ZP_BASE_APPLICATION_FLAG_IS_FOCUSED =       1ULL << 2,

    ZP_BASE_APPLICATION_FLAG_SHOULD_RESTART =               1ULL << 3,
    ZP_BASE_APPLICATION_FLAG_SHOULD_GARBAGE_COLLECT =       1ULL << 4,
    ZP_BASE_APPLICATION_FLAG_SHOULD_RELOAD_ALL_RESOURCES =  1ULL << 5,
    ZP_BASE_APPLICATION_FLAG_SHOULD_PAUSE_IN_BACKGROUND =   1ULL << 6,

    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_STATS =          1ULL << 32,
};

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
        } break;

        case WM_KILLFOCUS:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->setApplicationFocus( false );
        } break;

        case WM_SETFOCUS:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->setApplicationFocus( true );
        } break;

        case WM_KEYDOWN:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setKeyState( wParam, false );
        } break;

        case WM_KEYUP:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setKeyState( wParam, true );
        } break;

        case WM_MOUSEMOVE:
        {
            POINTS p = MAKEPOINTS( lParam );
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseLocationState( p.x, p.y );
        } break;

        case WM_MOUSEWHEEL:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseScrollWheelState( GET_WHEEL_DELTA_WPARAM( wParam ) > 0 ? 1 : -1 );
        } break;

        case WM_LBUTTONDOWN:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_LEFT, false );
        } break;

        case WM_LBUTTONUP:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_LEFT, true );
        } break;

        case WM_RBUTTONDOWN:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_RIGHT, false );
        } break;

        case WM_RBUTTONUP:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_RIGHT, true );
        } break;

        case WM_MBUTTONDOWN:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_MIDDLE, false );
        } break;

        case WM_MBUTTONUP:
        {
            zpBaseApplication* app = reinterpret_cast<zpBaseApplication*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
            app->getInput()->setMouseButtonState( ZP_MOUSE_BUTTON_MIDDLE, true );
        } break;
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
    , m_flags( 0 )
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
    ZP_PROFILER_BLOCK();
    
    onPreInitialize();

    createWindow();

    onPostInitialize();
}

zpMaterialHandle tm;
zpFontHandle ff;

void zpBaseApplication::setup()
{
    ZP_PROFILER_BLOCK();

    onPreSetup();

    m_renderingEngine.setup( m_hWnd );
    m_textureManager.setup( &m_renderingEngine );
    m_shaderManager.setup( &m_renderingEngine );
    m_materialManager.setup( &m_shaderManager, &m_textureManager );
    m_fontManager.setup( &m_materialManager );
    m_meshManager.setup();

    m_materialManager.getMaterial( "tempMaterial", tm );
    tm->color = { 1, 1, 1, 1 };
    tm->mainTexST = { 1, 1, 0, 0 };
    m_textureManager.loadTexture( "Assets/uv_checker_large.bmp", tm->mainTex );

    m_fontManager.getFont( "debug.font", ff );
    m_materialManager.getMaterial( "font.material", ff->fontMaterial );
    ff->fontMaterial->color = { 1, 1, 1, 1 };
    ff->fontMaterial->mainTexST = { 1, 1, 0, 0 };
    m_textureManager.loadTexture( "Assets/cp437_12x12.tga", ff->fontMaterial->mainTex );

    // TODO: remove when done debugging
    const zp_size_t fixedWidth = 12;
    const zp_size_t fixedHeight = 12;
    zp_uint tw = ff->fontMaterial->mainTex->width;
    zp_uint th = ff->fontMaterial->mainTex->height;

    float invW = 1.f / static_cast<zp_float>( tw );
    float invH = 1.f / static_cast<zp_float>( th );

    zp_float fw = fixedWidth  * invW;
    zp_float fh = fixedHeight * invH;

    zp_size_t c = 0;
    zp_memset( ff->fontGlyphs, 0, sizeof( ff->fontGlyphs ) );
    for( zp_uint h = 0; h < th; h += fixedHeight )
    {
        for( zp_uint w = 0; w < tw; w += fixedWidth )
        {
            zpGlyph* g = ff->fontGlyphs + c;
            g->uvRect.x =       w * invW;
            g->uvRect.y =       1.0f - ( h * invH ) - fh;
            g->uvRect.width =   fw;
            g->uvRect.height =  fh;
            g->size = { fixedWidth, fixedHeight };
            g->bearing = { fixedWidth, fixedHeight };
            g->advance = fixedWidth;
            g->baseline = 0;

            ++c;
        }
    }

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
    ZP_PROFILER_BLOCK();
    
    onPreTeardown();
    tm.release();
    ff.release();
    runGarbageCollection();

    m_meshManager.teardown();
    m_fontManager.teardown();
    m_materialManager.teardown();
    m_shaderManager.teardown();
    m_textureManager.teardown();
    m_renderingEngine.teardown();

    onPostTeardown();
}

zpApplicationExitCode zpBaseApplication::shutdown()
{
    ZP_PROFILER_BLOCK();
    
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

zpInput* zpBaseApplication::getInput()
{
    return &m_input;
}

void zpBaseApplication::runGarbageCollection()
{
    ZP_PROFILER_BLOCK();

    m_objectManager.garbageCollect();
    m_transformComponentManager.garbageCollect();

    m_meshManager.garbageCollect();
    m_fontManager.garbageCollect();
    m_materialManager.garbageCollect();
    m_textureManager.garbageCollect();
    m_shaderManager.garbageCollect();

    onGarbageCollection();
}

void zpBaseApplication::runReloadAllResources()
{
    ZP_PROFILER_BLOCK();

    onReloadAllResources();
}

void zpBaseApplication::runReloadChangedResources()
{
    ZP_PROFILER_BLOCK();

    m_meshManager.reloadChangedMeshs();
    m_fontManager.reloadChangedFonts();
    m_materialManager.reloadChangedMaterials();
    m_textureManager.reloadChangedTextures();

    onReloadChangedResources();
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
    ZP_PROFILER_START( ProcessFrame );

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

    runReloadChangedResources();

    ZP_PROFILER_START( InputPoll );
    m_input.poll();
    ZP_PROFILER_END( InputPoll );

    ZP_PROFILER_START( HandleInput );
    onHandleInput();
    ZP_PROFILER_END( HandleInput );

    ZP_PROFILER_START( Update );
    onUpdate( dt, rt );
    ZP_PROFILER_END( Update );

    ZP_PROFILER_START( TransformUpdate );
    m_transformComponentManager.update( dt, rt );
    ZP_PROFILER_END( TransformUpdate );

    ZP_PROFILER_START( LateUpdate );
    onLateUpdate( dt, rt );
    ZP_PROFILER_END( LateUpdate );

    zpRecti orthoRect = { 0, 0, 960, 640 };
    zpScalar l = zpMath::Scalar( (zp_float)orthoRect.x );
    zpScalar r = zpMath::Scalar( (zp_float)orthoRect.x + orthoRect.width );
    zpScalar t = zpMath::Scalar( (zp_float)orthoRect.y );
    zpScalar b = zpMath::Scalar( (zp_float)orthoRect.y + orthoRect.height );
    zpScalar n = zpMath::Scalar( -10 );
    zpScalar f = zpMath::Scalar( 10 );

    zpMatrix4f projection = zpMath::OrthoLH( l, r, t, b, n, f );

    zpRenderingContext *ctx = m_renderingEngine.getImmidiateContext();
    ctx->setViewport( { 0, 0, 960, 640, 1, 100 } );
    ctx->clear( { 0.2058f, 0.3066f, 0.4877f, 1.0f }, 1, 0 );
    zpRectf rect = { 100, 100, 300, 300 };
    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV );
    ctx->setMaterial( tm );
    ctx->setTransform( projection );
    ctx->addVertexData( { rect.x,               rect.y, 0, 1 },               { 1, 0, 0, 1 }, { 0, 1 } );
    ctx->addVertexData( { rect.x,               rect.y + rect.height, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0 } );
    ctx->addVertexData( { rect.x + rect.height, rect.y + rect.height, 0, 1 }, { 0, 0, 1, 1 }, { 1, 0 } );
    ctx->addVertexData( { rect.x + rect.height, rect.y, 0, 1 },               { 1, 1, 1, 1 }, { 1, 1 } );
    ctx->addQuadIndex( 0, 1, 2, 3 );
    ctx->endDrawImmediate();
    
#if 1
    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV );
    ctx->setTransform( zpMath::MatrixT( { .5f, -.5f, 0, 1 } ) );
    ctx->setMaterial( tm );
    ctx->setTransform( projection );
    ctx->addVertexData( { -1, 0, 0, 1 }, { 1, 0, 0, 1 }, { 0, 0 } );
    ctx->addVertexData( { -1, 1, 0, 1 }, { 0, 1, 0, 1 }, { 0, 1 } );
    ctx->addVertexData( { 0, 1, 0, 1 }, { 0, 0, 1, 1 }, { 1, 1 } );
    ctx->addVertexData( { 0, 0, 0, 1 }, { 1, 1, 1, 1 }, { 1, 0 } );
    ctx->addQuadIndex( 0, 1, 2, 3 );
    ctx->endDrawImmediate();

    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );
    ctx->setMaterial( tm );
    ctx->setTransform( projection );
    ctx->addVertexData( {  0,  1, 0, 1 }, { 1, 0, 0, 1 } );
    ctx->addVertexData( {  1,  0, 0, 1 }, { 0, 1, 0, 1 } );
    ctx->addVertexData( {  0,  0, 0, 1 }, { 0, 0, 1, 1 } );
    ctx->addTriangleIndex( 0, 1, 2 );
    ctx->endDrawImmediate();
    
    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );
    ctx->setMaterial( tm );
    ctx->setTransform( projection );
    ctx->addVertexData( { -1,  0, 0, 1 }, { 1, 0, 0, 1 } );
    ctx->addVertexData( { 0,  -1, 0, 1 }, { 0, 1, 0, 1 } );
    ctx->addVertexData( { -1,  -1, 0, 1 }, { 0, 0, 1, 1 } );
    ctx->addTriangleIndex( 0, 1, 2 );
    ctx->endDrawImmediate();
#endif

    zp_char buff[ 512 ];
    ctx->beginDrawText( 0, ff );
    ctx->setTransform( projection );
    const zpProfilerFrame* bf = g_profiler.getPreviousFrameBegin();
    const zpProfilerFrame* ef = g_profiler.getPreviousFrameEnd();

    zp_float y = 5;
    const zpVector2i& mpos = m_input.getMouseLocation();
    zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%d, %d", mpos.x, mpos.y );
    ctx->addText( { 5, y, 0, 1 }, buff, 12, {1, 1, 1, 1}, {1,1,1,1} );
    y += 12;

    for( ; bf != ef; ++bf )
    {
        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%s@%s %.3f", bf->functionName, bf->eventName, ( bf->endTime - bf->startTime ) * 1000.0f * m_time.getSecondsPerTick() );
        ctx->addText( { 5, y, 0, 1 }, buff, 12, { 100, 1, 1, 1 }, { 1, 1, 1, 1 } );
        y += 12;
    }
    ctx->endDrawText();

    
    m_renderingEngine.present();

    ZP_PROFILER_END( ProcessFrame );

    ++m_frameCount;

    ZP_PROFILER_START( Sleep );

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

    ZP_PROFILER_END( Sleep );

    //zp_printfln( "MS: %f", d );
    ZP_PROFILER_FINALIZE();
}
