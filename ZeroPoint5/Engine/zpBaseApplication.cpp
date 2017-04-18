#include "zpEngine.h"

enum zpBaseApplicationFlags
{
    ZP_BASE_APPLICATION_FLAG_IS_RUNNING,
    ZP_BASE_APPLICATION_FLAG_IS_PAUSED,
    ZP_BASE_APPLICATION_FLAG_IS_FOCUSED,

    ZP_BASE_APPLICATION_FLAG_SHOULD_RESTART,
    ZP_BASE_APPLICATION_FLAG_SHOULD_GARBAGE_COLLECT,
    ZP_BASE_APPLICATION_FLAG_SHOULD_RELOAD_ALL_RESOURCES,
    ZP_BASE_APPLICATION_FLAG_SHOULD_PAUSE_IN_BACKGROUND,

    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS,
    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS,

    zpBaseApplicationFlags_Count,
    zpBaseApplicationFlags_Force32 = ZP_FORECE_32BIT,

    ZP_BASE_APPLICATION_FLAG_IS_DEBUG_ACTIVE = ( 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS ) |
                                               ( 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS )
};

#define ZP_WINDOW_CLASS_NAME "zpWindowClass"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static LRESULT CALLBACK _WinProc( HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
    switch( uMessage )
    {
        case WM_CLOSE:
            PostQuitMessage( 0 );
            break;

        case WM_GETMINMAXINFO:
        {
            LPMINMAXINFO lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);

            POINT p = { 300, 300 };
            lpMMI->ptMinTrackSize = p;
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
zpObjectHandle ooo;
zpCameraHandle cam;

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
    m_cameraManager.setup();

    zpVector4fData st = { 1, 1, 0, 0 };

    m_materialManager.getMaterial( "tempMaterial", tm );
    tm->color = zpColor::White;
    tm->mainTexST = st;
    m_textureManager.loadTexture( "Assets/uv_checker_large.bmp", tm->mainTex );

    m_fontManager.getFont( "debug.font", ff );
    m_materialManager.getMaterial( "font.material", ff->fontMaterial );
    ff->fontMaterial->color = zpColor::White;
    ff->fontMaterial->mainTexST = st;
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
    
    zpVector2i s = { fixedWidth, fixedHeight };

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
            g->size = s;
            g->bearing = s;
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
    ooo.release();
    cam.release();
    runGarbageCollection();

    m_cameraManager.teardown();
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

    m_sceneManager.garbageCollect();
    m_objectManager.garbageCollect();
    m_transformComponentManager.garbageCollect();
    m_particleEmitterComponentManager.garbageCollect();

    m_cameraManager.garbageCollect();
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

#ifdef ZP_USE_HOT_RELOAD
    // hot reload resources
    runReloadChangedResources();
#endif

    // handle input
    handleInput();

    // update
    update( dt, rt );

    // render
    render();

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

    zp_sleep( static_cast<zp_int>( sleepTime + 0.5f ) );

    ZP_PROFILER_END( Sleep );

    //zp_printfln( "MS: %f", d );
    ZP_PROFILER_FINALIZE();
}

void zpBaseApplication::handleInput()
{
    ZP_PROFILER_BLOCK();

    ZP_PROFILER_START( InputPoll );
    m_input.poll();
    ZP_PROFILER_END( InputPoll );

    ZP_PROFILER_START( HandleInput );
    onHandleInput();
    ZP_PROFILER_END( HandleInput );

    if( m_input.isKeyPressed( ZP_KEY_CODE_ESC ) )
    {
        exit( ZP_APPLICATION_EXIT_NORMAL );
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F5 ) )
    {
        restart();
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_G ) )
    {
        garbageCollect();
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F1 ) )
    {
        m_flags ^= 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F2 ) )
    {
        m_flags ^= 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_O ) )
    {
        m_objectManager.createObject( ooo );
        ooo->setName( "Test Object" );

        m_transformComponentManager.createTransformComponent( ooo->getAllComponents()->transform );
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_C ) )
    {
        m_cameraManager.createCamera( cam );
        cam->projectionType = ZP_CAMERA_PROJECTION_PERSPECTIVE;
        cam->flags |= 0xFF;
        cam->clearMode = ZP_CAMERA_CLEAR_MODE_DEFAULT;
    }
}

void zpBaseApplication::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    ZP_PROFILER_START( Update );
    onUpdate( dt, rt );
    ZP_PROFILER_END( Update );

    ZP_PROFILER_START( TransformUpdate );
    m_transformComponentManager.update( dt, rt );
    ZP_PROFILER_END( TransformUpdate );

    ZP_PROFILER_START( ParticleEmitterUpdate );
    m_particleEmitterComponentManager.update( dt, rt );
    ZP_PROFILER_END( ParticleEmitterUpdate );

    ZP_PROFILER_START( CameraUpdate );
    m_cameraManager.update( dt, rt );
    ZP_PROFILER_END( CameraUpdate );

    ZP_PROFILER_START( LateUpdate );
    onLateUpdate( dt, rt );
    ZP_PROFILER_END( LateUpdate );
}

void zpBaseApplication::render()
{
    ZP_PROFILER_BLOCK();

    zpRecti orthoRect = { 0, 0, 960, 640 };
    zp_float l = static_cast<zp_float>( orthoRect.x );
    zp_float r = static_cast<zp_float>( orthoRect.x + orthoRect.width );
    zp_float t = static_cast<zp_float>( orthoRect.y );
    zp_float b = static_cast<zp_float>( orthoRect.y + orthoRect.height );
    zp_float n = static_cast<zp_float>( -10 );
    zp_float f = static_cast<zp_float>( 10 );

    zp_float fovy =  ( 45.f );
    zp_float ratio = ( static_cast<zp_float>( orthoRect.width ) / static_cast<zp_float>( orthoRect.height ) );
    zp_float zn =    ( 1 );
    zp_float zf =    ( 100 );

    zpVector4f cen = zpMath::Vector4( 0, 0, 0, 1 );
    zpVector4f eye = zpMath::Vector4( 20, 10, -30, 1 );
    zpVector4f dir = zpMath::Vector4Normalize3( zpMath::Vector4Sub( cen, eye ) );
    zpVector4f up = zpMath::Vector4( 0, 1, 0, 0 );

    zpMatrix4f ortho = zpMath::OrthoLH( l, r, t, b, n, f );
    zpMatrix4f proj = zpMath::PerspectiveLH( fovy, ratio, zn, zf );
    zpMatrix4f view = zpMath::LookAtLH( eye, dir, up );

    zpViewport vp = { 0, 0, 960, 640, 1, 100 };

    zpColorf clearColor = { 0.2058f, 0.3066f, 0.4877f, 1.0f };

    zpRectf rect = { -10, -10, 30, 30 };
    zpVector4fData v0 =  { rect.x,               rect.y, 0, 1 };
    zpVector4fData v1 =  { rect.x,               rect.y + rect.height, 0, 1 };
    zpVector4fData v2 =  { rect.x + rect.height, rect.y + rect.height, 0, 1 };
    zpVector4fData v3 =  { rect.x + rect.height, rect.y, 0, 1 };

    zpVector2f uv0 = { 0, 0 };
    zpVector2f uv1 = { 0, 1 };
    zpVector2f uv2 = { 1, 1 };
    zpVector2f uv3 = { 1, 0 };

    zpRenderingContext *ctx = m_renderingEngine.getImmidiateContext();
    ctx->setViewport( vp );
    ctx->clear( clearColor, 1, 0 );

    ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV );
    ctx->setMaterial( tm );
    ctx->setTransform( zpMath::MatrixMul( view, proj ) );
    ctx->addVertexData( v0, zpColor32::Red, uv0 );
    ctx->addVertexData( v1, zpColor32::Green, uv1 );
    ctx->addVertexData( v2, zpColor32::Blue, uv2 );
    ctx->addVertexData( v3, zpColor32::White, uv3 );
    ctx->addQuadIndex( 0, 1, 2, 3 );
    ctx->endDraw();

    // draw debug when toggled on
    if( m_flags & ZP_BASE_APPLICATION_FLAG_IS_DEBUG_ACTIVE )
    {
        debugDrawGUI();
    }

    ZP_PROFILER_START( Present );
    m_renderingEngine.present();
    ZP_PROFILER_END( Present );
}

void zpBaseApplication::debugDrawGUI()
{
    ZP_PROFILER_BLOCK();

    zpRenderingContext *ctx = m_renderingEngine.getImmidiateContext();

    zpRecti orthoRect = { 0, 0, 960, 640 };
    zp_float l = static_cast<zp_float>( orthoRect.x );
    zp_float r = static_cast<zp_float>( orthoRect.x + orthoRect.width );
    zp_float t = static_cast<zp_float>( orthoRect.y );
    zp_float b = static_cast<zp_float>( orthoRect.y + orthoRect.height );
    zp_float n = static_cast<zp_float>( -10 );
    zp_float f = static_cast<zp_float>( 10 );

    zpMatrix4f ortho = zpMath::OrthoLH( l, r, t, b, n, f );

    const zp_uint fontHeight = 12;
    const zp_uint fontSpacing = 4;
    zp_float y = 5;
    zpVector4fData tp = { 5, y, 0, 1 };

    zp_char buff[ 512 ];

    ctx->beginDrawText( 0, ff );
    ctx->setTransform( ortho );

    if( m_flags & ( 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS ) )
    {
        const zpProfilerFrame* bf = g_profiler.getPreviousFrameBegin();
        const zpProfilerFrame* ef = g_profiler.getPreviousFrameEnd();
        
        tp.y = y;

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6s %8s %s", "Ms", "Mem", "Function" );
        ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
        y += fontHeight + fontSpacing;

        for( ; bf != ef; ++bf )
        {
            zp_float ft = ( ( bf->endTime - bf->startTime ) * static_cast<zp_time_t>( 1000 ) ) * m_time.getSecondsPerTick();
            zp_size_t fm = ( bf->endMemory - bf->startMemory );

            tp.y = y;

            if( bf->eventName )
            {
                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6.3f %8Iu %s@%s", ft, fm, bf->functionName, bf->eventName );
            }
            else
            {
                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6.3f %8Iu %s", ft, fm, bf->functionName );
            }
            ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
            y += fontHeight + fontSpacing;
        }

        y += fontHeight + fontSpacing;

        tp.y = y;

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6s %8s", "Mem", "Allocs" );
        ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
        y += fontHeight + fontSpacing;

        tp.y = y;

        zp_size_t memUsed = g_globalAllocator.getMemoryUsed();
        zp_float mem = 0.f;
        zp_char memSize = 'U';

        if( memUsed > ZP_MEMORY_MB( 1 ) )
        {
            mem = static_cast<zp_float>( memUsed ) / static_cast<zp_float>( ZP_MEMORY_MB( 1 ) );
            memSize = 'M';
        }
        else
        {
            mem = static_cast<zp_float>( memUsed ) / static_cast<zp_float>( ZP_MEMORY_KB( 1 ) );
            memSize = 'K';
        }

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6.3f%c %7Iu", mem, memSize, g_globalAllocator.getNumAllocations() );
        ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
        y += fontHeight + fontSpacing;
    }
    
    if( m_flags & ( 1 << ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS ) )
    {
        const zpObjectInstance* const* bo = m_objectManager.beginActiveObjects();
        const zpObjectInstance* const* eo = m_objectManager.endActiveObjects();
        
        tp.y = y;

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "Objects (%Iu)", ( eo - bo ) );
        ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
        y += fontHeight + fontSpacing;

        for( ; bo != eo; ++bo )
        {
            const zpObjectInstance* inst = (*bo);
            const zpObject* obj = &inst->object;

            tp.y = y;

            zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "- '%s'@%lu (%Iu)", obj->getName().str(), obj->getInstanceId(), inst->refCount );
            ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
            y += fontHeight + fontSpacing;

            const zpTransformComponent* txn = obj->getAllComponents()->transform.get();
            if( txn )
            {
                tp.y = y;

                const zpVector4fData& localPos = txn->getLocalPosition();

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "--- 'Txn'@%lu [%3.3f %3.3f %3.3f]", txn->getInstanceId(), localPos.x, localPos.y, localPos.z );
                ctx->addText( tp, buff, fontHeight, zpColor32::White, zpColor32::Grey75 );
                y += fontHeight + fontSpacing;
            }
        }

        y += fontHeight + fontSpacing;
    }

    ctx->endDraw();
}
