#include "Engine/zpEngine.h"

enum zpBaseApplicationFlags
{
    ZP_BASE_APPLICATION_FLAG_IS_RUNNING,
    ZP_BASE_APPLICATION_FLAG_IS_PAUSED,
    ZP_BASE_APPLICATION_FLAG_IS_FOCUSED,

    ZP_BASE_APPLICATION_FLAG_SHOULD_RESTART,
    ZP_BASE_APPLICATION_FLAG_SHOULD_GARBAGE_COLLECT,
    ZP_BASE_APPLICATION_FLAG_SHOULD_RELOAD_ALL_RESOURCES,
    ZP_BASE_APPLICATION_FLAG_SHOULD_PAUSE_IN_BACKGROUND,

    zpBaseApplicationFlags_Count,
    zpBaseApplicationFlags_Force32 = ZP_FORCE_32BIT,
};

enum
{
    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_FRAME_STATS =    1 << 0,
    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS = 1 << 1,
    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_TIMELINE_STATS = 1 << 2,
    ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS =   1 << 3,
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
    , m_flags( 0 )
    , m_debugFlags( 0 )
    , m_screenSize( { 960, 640 } )
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
zpMaterialHandle dm;
zpFontHandle ff;
zpMeshHandle mh;
zpCameraHandle cam;
zpSceneHandle sc;

void zpBaseApplication::setup()
{
    ZP_PROFILER_BLOCK();
    
    onPreSetup();

    m_sceneManager.setup();
    m_objectManager.setup();

    m_renderingEngine.setup( m_hWnd );
    m_textureManager.setup( &m_renderingEngine );
    m_shaderManager.setup( &m_renderingEngine );
    m_materialManager.setup( &m_shaderManager, &m_textureManager );
    m_fontManager.setup( &m_materialManager );
    m_meshManager.setup();
    m_cameraManager.setup();

    m_materialManager.getMaterial( "tempMaterial", tm );
    tm->setColor( "_Color", zpColor::White );
    tm->setVector( "_MainTex_ST", { 1, 1, 0, 0 } );
    //tm->color = zpColor::White;
    //tm->mainTexST = { 1, 1, 0, 0 };
    zpTextureHandle tempTex;
    m_textureManager.loadTexture( "Assets/uv_checker_large.bmp", tempTex );
    tm->setTexture( "_MainTex", tempTex );

    m_materialManager.getMaterial( "debug_material", dm );
    dm->setColor( "_Color", zpColor::White );

    zpTextureHandle fontTex;
    m_fontManager.getFont( "debug.font", ff );
    m_materialManager.getMaterial( "font.material", ff->fontMaterial );
    //ff->fontMaterial->color = zpColor::White;
    //ff->fontMaterial->mainTexST = { 1, 1, 0, 0 };
    ff->fontMaterial->setColor( "_Color", zpColor::White );
    ff->fontMaterial->setVector( "_MainTex_ST", { 1, 1, 0, 0 } );
    m_textureManager.loadTexture( "Assets/cp437_12x12.tga", fontTex );
    ff->fontMaterial->setTexture( "_MainTex", fontTex );

    m_meshManager.getMesh( "default.mesh", mh );
    m_sceneManager.createScene( sc );

    m_debugGUI.setup( &m_input, ff, dm );

    m_cameraManager.createCamera( cam );
    cam->projectionType = ZP_CAMERA_PROJECTION_PERSPECTIVE;
    cam->position = { 0, 0, 10, 1 };
    cam->forward = { 0, 0, -1, 0 };
    cam->up = { 0, 1, 0, 0 };
    cam->flags |= 0xFF;
    cam->clearMode = ZP_CAMERA_CLEAR_MODE_DEFAULT;
    cam->clearColor = zpColor::Blue;
    cam->clearDepth = 0;
    cam->clearStencil = 0;
    cam->fovy = 45.f;
    cam->aspectRatio = static_cast<zp_float>( m_screenSize.x ) / static_cast<zp_float>( m_screenSize.y );
    cam->zNear = 0.f;
    cam->zFar = 1.f;
    cam->viewport = { 0, 0, m_screenSize.x, m_screenSize.y, 0, 1 };
    cam->clipRect = { 0, 0, m_screenSize.x, m_screenSize.y };

    // TODO: remove when done debugging
    const zp_size_t fixedWidth = 12;
    const zp_size_t fixedHeight = 12;
    zp_uint tw = fontTex->desc.width;
    zp_uint th = fontTex->desc.height;

    float invW = 1.f / static_cast<zp_float>( tw );
    float invH = 1.f / static_cast<zp_float>( th );

    zp_float fw = fixedWidth  * invW;
    zp_float fh = fixedHeight * invH;
    
    zpVector2i s = { fixedWidth, fixedHeight };

    zp_size_t c = 0;
    ff->baseFontSize = 12;
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

    m_debugGUI.teardown();

    tm.release();
    dm.release();
    ff.release();
    cam.release();
    mh.release();
    sc.release();

    runGarbageCollection();

    m_objectManager.teardown();
    m_sceneManager.teardown();
    m_renderingEngine.teardown();

    runGarbageCollection();

    m_cameraManager.teardown();
    m_meshManager.teardown();
    m_fontManager.teardown();
    m_materialManager.teardown();
    m_shaderManager.teardown();
    m_textureManager.teardown();

    runGarbageCollection();

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
    m_rectTransformComponentManager.garbageCollect();
    m_particleEmitterComponentManager.garbageCollect();
    m_meshRendererComponentManager.garbageCollect();

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
    ZP_PROFILER_BLOCK();

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
    zpRecti windowRect = { CW_USEDEFAULT, CW_USEDEFAULT, m_screenSize.x, m_screenSize.y };

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
    ZP_PROFILER_INITIALIZE();

    ZP_PROFILER_START( ProcessFrame );

    zp_bool paused = m_isPaused || ( m_shouldPauseInBackground && !m_isFocused );

    zpTime::get().tick();

    zp_long startTime = zpTime::get().getTime();
    zp_float dt = paused ? 0.f : zpTime::get().getDeltaSeconds();
    zp_float rt = zpTime::get().getActualDeltaSeconds();

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
    
    // late update
    lateUpdate( dt, rt );

    // render
    render();

    ZP_PROFILER_END( ProcessFrame );

    ZP_PROFILER_START( Sleep );

    zp_long endTime = zpTime::get().getTime();

    zp_float spf = 1.f / static_cast<zp_float>( m_targetFps );
    zp_long diff = ( endTime - startTime ) * 1000LL;
    zp_float d = diff * zpTime::get().getSecondsPerTick();
    zp_float sleepTime = ( 1000.f * spf ) - d;
    while( sleepTime < 0.f )
    {
        sleepTime += spf;
    }

    zp_sleep( static_cast<zp_int>( sleepTime ) );

    ZP_PROFILER_END( Sleep );

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
        m_debugFlags ^= ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_FRAME_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F2 ) )
    {
        m_debugFlags ^= ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F3 ) )
    {
        m_debugFlags ^= ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_TIMELINE_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_F4 ) )
    {
        m_debugFlags ^= ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS;
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_O ) )
    {
        zpObjectHandle ooo;

        m_objectManager.createObject( ooo );
        sc->addObject( ooo );

        ooo->setName( "Test Object" );

        m_transformComponentManager.createTransformComponent( ooo->getAllComponents()->transform, ZP_NULL );
        ooo->getAllComponents()->transform->setParentObject( ooo );
        ooo->getAllComponents()->transform->setEnabled( true );

        m_meshRendererComponentManager.createMeshRendererComponent( ooo->getAllComponents()->meshRenderer, ZP_NULL );
        ooo->getAllComponents()->meshRenderer->setParentObject( ooo );
        ooo->getAllComponents()->meshRenderer->setMesh( mh );
        ooo->getAllComponents()->meshRenderer->setMaterial( tm );
        ooo->getAllComponents()->meshRenderer->setEnabled( true );
    }
    else if( m_input.isKeyPressed( ZP_KEY_CODE_U ) )
    {
        zpStackTrace::Dump();
    }
}

void zpBaseApplication::update( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();

    ZP_PROFILER_START( OnUpdate );
    onUpdate( dt, rt );
    ZP_PROFILER_END( OnUpdate );

    ZP_PROFILER_START( OnComponentUpdate );
    m_transformComponentManager.update( dt, rt );

    m_rectTransformComponentManager.update( dt, rt );

    m_particleEmitterComponentManager.update( dt, rt );

    m_meshRendererComponentManager.update( dt, rt );

    m_cameraManager.update( dt, rt );
    ZP_PROFILER_END( OnComponentUpdate );

}

void zpBaseApplication::lateUpdate( zp_float dt, zp_float rt )
{
    ZP_PROFILER_BLOCK();
  
    ZP_PROFILER_START( OnLateUpdate );
    onLateUpdate( dt, rt );
    ZP_PROFILER_END( OnLateUpdate );

    ZP_PROFILER_START( OnComponentLateUpdate );
    m_transformComponentManager.lateUpdate( dt, rt );

    m_rectTransformComponentManager.lateUpdate( dt, rt );

    //m_particleEmitterComponentManager.lateUpdate( dt, rt );

    m_meshRendererComponentManager.lateUpdate( dt, rt );

    //m_cameraManager.lateUpdate( dt, rt );
    ZP_PROFILER_END( OnComponentLateUpdate );
}

void zpBaseApplication::render()
{
    ZP_PROFILER_BLOCK();

    zp_float fovy =  ( 45.f );
    zp_float ratio = ( static_cast<zp_float>( m_screenSize.x ) / static_cast<zp_float>( m_screenSize.y ) );
    zp_float zn =    ( 0 );
    zp_float zf =    ( 1 );

    zpVector4fSimd cen = zpMath::Vector4( 0, 0, 0, 1 );
    zpVector4fSimd eye = zpMath::Vector4( 20, 10, 30, 1 );
    zpVector4fSimd dir = zpMath::Vector4Normalize3( zpMath::Vector4Sub( cen, eye ) );
    zpVector4fSimd up = zpMath::Vector4( 0, 1, 0, 0 );

    zpMatrix4fSimd proj = zpMath::PerspectiveReverseZLH( fovy, ratio, zn );
    zpMatrix4fSimd view = zpMath::LookAtLH( eye, dir, up );
    zpMatrix4fSimd viewProj = zpMath::MatrixMul( view, proj );

    zpViewport vp = { 0, 0, m_screenSize.x, m_screenSize.y, zn, zf };

    zpColorf clearColor = { 0.2058f, 0.3066f, 0.4877f, 1.0f };

    zpRectf rect = { -10, -10, 30, 30 };
    zpVector4f v0 =  { rect.x,               rect.y + rect.height, 0, 1 };
    zpVector4f v1 =  { rect.x,               rect.y, 0, 1 };
    zpVector4f v2 =  { rect.x + rect.height, rect.y, 0, 1 };
    zpVector4f v3 =  { rect.x + rect.height, rect.y + rect.height, 0, 1 };

    zpVector2f uv0 = { 0, 1 };
    zpVector2f uv1 = { 0, 0 };
    zpVector2f uv2 = { 1, 0 };
    zpVector2f uv3 = { 1, 1 };

    zpRenderContext* ctx = m_renderingEngine.getContext();
    //zpRenderingContext *ctx = m_renderingEngine.getImmidiateContext();
    //ctx->setViewport( vp );
    //ctx->clear( clearColor, 1, 0 );

    //ctx->beginDrawImmediate( 0, ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, tm );
    //ctx->setTransform( viewProj );
    //ctx->addVertexData( v0, zpColor32::Red, uv0 );
    //ctx->addVertexData( v1, zpColor32::Green, uv1 );
    //ctx->addVertexData( v2, zpColor32::Blue, uv2 );
    //ctx->addVertexData( v3, zpColor32::White, uv3 );
    //ctx->addQuadIndex( 0, 1, 2, 3 );
    //ctx->endDraw();

    m_renderingEngine.beginFrame( zpTime::get().getFrameCount() );

    m_meshRendererComponentManager.render( ctx );

    auto imm = m_renderingEngine.getImmediate();
    imm->beginDrawImmediate( ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR );
    imm->setMaterial( dm );
    imm->setTransform( viewProj );
    v0 = {  10.5f,  10.5f, 0.f, 1.f };
    v1 = {  10.5f, -10.5f, 0.f, 1.f };
    v2 = { -10.5f, -10.5f, 0.f, 1.f };
    v3 = { -10.5f,  10.5f, 0.f, 1.f };
    imm->addVertex( v0, { 255,0,0,255 } );
    imm->addVertex( v1, { 0,255,0,255 } );
    imm->addVertex( v2, { 0,0,255,255 } );
    imm->addVertex( v3, { 255,255,255,255 } );
    imm->addQuadIndex( 0, 1, 2, 3 );
    imm->endDrawImmediate();

    // draw debug when toggled on
    if( m_debugFlags )
    {
        debugDrawGUI();
    }
    
    m_cameraManager.render( &m_renderingEngine );

    m_renderingEngine.endFrame();

    m_renderingEngine.submit();
}

void zpBaseApplication::debugDrawGUI()
{
    ZP_PROFILER_BLOCK();

    zp_char buff[ 512 ];

    m_debugGUI.startGUI();

#ifdef ZP_USE_PROFILER
    if( m_debugFlags & ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_FRAME_STATS )
    {
        ZP_PROFILER_FUNCTION( "FrameStats" );
        const zpProfilerFrameTimeline* tl = g_profiler.getPreviousFrameTimeline();
        
        zp_float pft = ( ( tl->frameEndTime - tl->frameStartTime ) * static_cast<zp_time_t>( 1000 ) ) * zpTime::get().getSecondsPerTick();
        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "Frame Time: %6.3f", pft );
        m_debugGUI.label( buff );

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "  GPU Time: %6.3f  Prim Count: %Iu", tl->gpuFrameTime / 1000000.f, tl->gpuPrimitiveCount );
        m_debugGUI.label( buff );
    }

    if( m_debugFlags & ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_PROFILER_STATS )
    {
        ZP_PROFILER_FUNCTION( "ProfilerStats" );
        const zpProfilerFrame* bf = g_profiler.getPreviousFrameBegin();
        const zpProfilerFrame* ef = g_profiler.getPreviousFrameEnd();

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6s %8s %8s %s", "Ms", "Cycles", "Mem", "Function" );
        m_debugGUI.label( buff );
        zp_int statsMax = 64;
        for( ; bf != ef && statsMax --> 0; ++bf )
        {
            zp_size_t parentFrame = bf->parentFrame;

            zp_int stackCount = 0;
            while( parentFrame != zpProfiler::npos )
            {
                parentFrame = ( g_profiler.getPreviousFrameBegin() + parentFrame )->parentFrame;
                stackCount++;
            }

            zp_float ft = ( ( bf->endTime - bf->startTime ) * static_cast<zp_time_t>( 1000 ) ) * zpTime::get().getSecondsPerTick();
            zp_ulong ct = ( bf->endCycles - bf->startCycles );
            zp_size_t fm = ( bf->endMemory - bf->startMemory );

            if( bf->eventName )
            {
                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6.3f %8Iu %8Iu %*s%s@%s", ft, ct, fm, stackCount, "", bf->functionName, bf->eventName );
            }
            else
            {
                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6.3f %8Iu %8Iu %*s%s", ft, ct, fm, stackCount, "", bf->functionName );
            }
            m_debugGUI.label( buff );
        }

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%6s %8s", "Mem", "Live Mem" );
        m_debugGUI.label( buff );

        zp_size_t memUsed = g_globalAllocator->getMemoryUsed();
        zp_float mem = 0.f;
        zp_char memSize = 'U';

        if( memUsed > ZP_MEMORY_GB( 1 ) )
        {
            mem = static_cast<zp_float>( memUsed ) / static_cast<zp_float>( ZP_MEMORY_GB( 1 ) );
            memSize = 'G';
        }
        else if( memUsed > ZP_MEMORY_MB( 1 ) )
        {
            mem = static_cast<zp_float>( memUsed ) / static_cast<zp_float>( ZP_MEMORY_MB( 1 ) );
            memSize = 'M';
        }
        else
        {
            mem = static_cast<zp_float>( memUsed ) / static_cast<zp_float>( ZP_MEMORY_KB( 1 ) );
            memSize = 'K';
        }

        const zp_ptrdiff_t liveMem = g_globalAllocator->getNumAllocations() - g_globalAllocator->getNumFrees();
        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "%5.3f%c %8d", mem, memSize, liveMem );
        m_debugGUI.label( buff );
    }

    if( m_debugFlags & ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_TIMELINE_STATS )
    {
        ZP_PROFILER_FUNCTION( "TimelineStats" );
        const zpProfilerFrame* bf;
        const zpProfilerFrame* ef = g_profiler.getPreviousFrameEnd();
        const zpProfilerFrameTimeline* tl = g_profiler.getPreviousFrameTimeline();

        zp_float totalTime = static_cast<zp_float>( tl->frameEndTime - tl->frameStartTime );

        const zp_int totalWidth = 320;
        const zp_int timelineHeight = 12;

        zp_float width = 0;
        m_debugGUI.beginHorizontal( totalWidth );
        m_debugGUI.beginVertical();

        m_debugGUI.box( 0, 0, totalWidth, timelineHeight );

        for( bf = g_profiler.getPreviousFrameBegin(); bf != ef; ++bf )
        {
            zp_int x = static_cast<zp_int>( totalWidth * static_cast<zp_float>( bf->startTime - tl->frameStartTime ) / totalTime );
            zp_int w = static_cast<zp_int>( totalWidth * static_cast<zp_float>( bf->endTime - bf->startTime ) / totalTime );

            m_debugGUI.box( x, 0, w, timelineHeight );
        }

        m_debugGUI.endVertical();
        m_debugGUI.beginVertical();

        m_debugGUI.label( "Frame Time" );

        for( bf = g_profiler.getPreviousFrameBegin(); bf != ef; ++bf )
        {
            m_debugGUI.label( bf->eventName ? bf->eventName : bf->functionName );
        }

        m_debugGUI.endVertical();
        m_debugGUI.endHorizontal();
    }

#endif

    if( m_debugFlags & ZP_BASE_APPLICATION_FLAG_DEBUG_DISPLAY_OBJECT_STATS )
    {
        ZP_PROFILER_FUNCTION( "ObjectStats" );
        const zpObjectInstance* const* bo = m_objectManager.beginActiveObjects();
        const zpObjectInstance* const* eo = m_objectManager.endActiveObjects();

        zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "Objects (%Iu)", ( eo - bo ) );
        m_debugGUI.label( buff );

        for( ; bo != eo; ++bo )
        {
            const zpObjectInstance* inst = ( *bo );
            const zpObject* obj = &inst->object;

            zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "- '%s'@%lu (%Iu)", obj->getName().str(), obj->getInstanceId(), inst->refCount );
            m_debugGUI.label( buff );

            const zpTransformComponent* txn = obj->getAllComponents()->transform.get();
            if( txn )
            {
                const zpVector4f& localPos = txn->getLocalPosition();
                const zpQuaternion4f& localRot = txn->getLocalRotation();
                const zpVector4f& localScale = txn->getLocalScale();

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "--- Transform@%lu", txn->getInstanceId() );
                m_debugGUI.label( buff );

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "    -Local Position [%3.3f %3.3f %3.3f %3.3f]", localPos.x, localPos.y, localPos.z, localPos.w );
                m_debugGUI.label( buff );

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "    -Local Rotation [%3.3f %3.3f %3.3f %3.3f]", localRot.x, localRot.y, localRot.z, localRot.w );
                m_debugGUI.label( buff );

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "    -Local Scale    [%3.3f %3.3f %3.3f %3.3f]", localScale.x, localScale.y, localScale.z, localScale.w );
                m_debugGUI.label( buff );
            }

            const zpParticleEmitterComponent* part = obj->getAllComponents()->particleEmitter.get();
            if( part )
            {
                //zp_bool a = part->isAnyPlaying();

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "--- Particle Emitter@%lu\n", part->getInstanceId() );
                m_debugGUI.label( buff );
            }

            const zpMeshRendererComponent* mesh = obj->getAllComponents()->meshRenderer.get();
            if( mesh )
            {
                //zp_bool a = part->isAnyPlaying();

                zp_snprintf( buff, sizeof( buff ), sizeof( buff ), "--- Mesh Renderer@%lu\n", mesh->getInstanceId() );
                m_debugGUI.label( buff );
            }
        }
    }

    m_debugGUI.endGUI();

    m_debugGUI.update( 0, 0 );

    zpRenderImmediate *ctx = m_renderingEngine.getImmediate();
    m_debugGUI.render( ctx );
}
