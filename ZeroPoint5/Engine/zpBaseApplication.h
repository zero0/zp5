#pragma once
#ifndef ZP_BASE_APPLICATION_H
#define ZP_BASE_APPLICATION_H

enum zpApplicationExitCode : zp_int
{
    ZP_APPLICATION_EXIT_NORMAL = 0,
    ZP_APPLICATION_EXIT_ERROR = 1,
};

ZP_ABSTRACT_CLASS zpBaseApplication
{
    ZP_NON_COPYABLE( zpBaseApplication );
public:
    zpBaseApplication();
    virtual ~zpBaseApplication();

    void processCommandLine( const zp_char* commandline );

    void initialize();
    void setup();
    void run();
    void teardown();
    zpApplicationExitCode shutdown();

    void setPaused( zp_bool paused );
    zp_bool isPaused() const;

    void restart();
    void exit( zpApplicationExitCode exitCode );

protected:
    virtual void onPreInitialize() = 0;
    virtual void onPostInitialize() = 0;

    virtual void onPreSetup() = 0;
    virtual void onPostSetup() = 0;

    virtual void onPreTeardown() = 0;
    virtual void onPostTeardown() = 0;

    virtual void onPreShutdown() = 0;
    virtual void onPostShutdown() = 0;


private:
    zp_bool processMessages();
    void processFrame();

    zp_handle m_hWnd;
    zp_handle m_hInstance;

    zpApplicationExitCode m_exitCode;

    zp_bool m_isRunning;
    zp_bool m_isRestart;
    zp_bool m_isPaused;
};

#endif // !ZP_BASE_APPLICATION_H
