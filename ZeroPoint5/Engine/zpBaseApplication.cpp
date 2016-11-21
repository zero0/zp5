#include "zpEngine.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

zpBaseApplication::zpBaseApplication()
    : m_hWnd( ZP_NULL )
    , m_hInstance( ZP_NULL )
    , m_exitCode( ZP_APPLICATION_EXIT_NORMAL )
    , m_isRunning( false )
    , m_isRestart( false )
    , m_isPaused( false )
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


void zpBaseApplication::initialize()
{
    onPreInitialize();

    onPostInitialize();
}

void zpBaseApplication::setup()
{
    onPreSetup();

    onPostSetup();
}

void zpBaseApplication::run()
{
    do
    {
        m_isRunning = true;
        m_isRestart = false;
 
        setup();

        while( m_isRunning && processMessages() )
        {
            processFrame();
        }

        teardown();
    } while( m_isRestart );
}

void zpBaseApplication::teardown()
{
    onPreTeardown();

    onPostTeardown();
}

zpApplicationExitCode zpBaseApplication::shutdown()
{
    onPreShutdown();

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
    m_isRestart = true;
    m_isRunning = false;
}

void zpBaseApplication::exit( zpApplicationExitCode exitCode )
{
    m_isRunning = false;
    m_exitCode = exitCode;
}

zp_bool zpBaseApplication::processMessages()
{
#ifdef ZP_WINDOWS
    MSG message;
    if( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) )
    {
        if( message.message == WM_QUIT )
        {
            return false;
        }

        TranslateMessage( &message );
        DispatchMessage( &message );
    }
#endif // ZP_WINDOWS

    return true;
}

void zpBaseApplication::processFrame()
{
    // TODO: implement
}
