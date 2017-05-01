#pragma once
#ifndef ZP_BASE_APPLICATION_H
#define ZP_BASE_APPLICATION_H

enum zpApplicationExitCode : zp_int
{
    ZP_APPLICATION_EXIT_NORMAL = 0,
    ZP_APPLICATION_EXIT_ERROR = 1,
};

//ZP_ABSTRACT_CLASS zpBaseApplication
class zpBaseApplication
{
    ZP_NON_COPYABLE( zpBaseApplication );
public:
    zpBaseApplication();
    virtual ~zpBaseApplication();

    void processCommandLine( const zp_char* commandline );
    void setHandle( zp_handle instance );

    void initialize();
    void setup();
    void run();
    void teardown();
    zpApplicationExitCode shutdown();

    void setPaused( zp_bool paused );
    zp_bool isPaused() const;

    void restart();
    void exit( zpApplicationExitCode exitCode );

    zp_bool shouldRestart() const;
    zp_bool isRunning() const;

    void garbageCollect();
    void reloadAllResouces();

    void setApplicationFocus( zp_bool focused );
    
    zpInput* getInput();

protected:
    virtual void onPreInitialize() {}
    virtual void onPostInitialize() {}

    virtual void onPreSetup() {}
    virtual void onPostSetup() {}

    virtual void onPreTeardown() {}
    virtual void onPostTeardown() {}

    virtual void onPreShutdown() {}
    virtual void onPostShutdown() {}

    virtual void onGarbageCollection() {}
    virtual void onReloadAllResources() {}
    virtual void onReloadChangedResources() {}

    virtual void onHandleInput() {}
    virtual void onUpdate( zp_float dt, zp_float rt ) {}
    virtual void onLateUpdate( zp_float dt, zp_float rt ) {}

    virtual void onFixedUpdate( zp_float ft, zp_float rt ) {}

private:
    void runGarbageCollection();
    void runReloadAllResources();
    void runReloadChangedResources();

    void createWindow();
    void destroyWindow();

    zp_bool processMessages();
    void processFrame();

    void handleInput();
    void update( zp_float dt, zp_float rt );
    void render();

    void debugDrawGUI();

    zpInput m_input;

    zpRenderingEngine m_renderingEngine;

    zpSceneManager m_sceneManager;
    zpObjectManager m_objectManager;

    zpTransformComponentManager m_transformComponentManager;
    zpParticleEmitterComponentManager m_particleEmitterComponentManager;
    zpMeshRendererComponentManager m_meshRendererComponentManager;

    zpTextureManager m_textureManager;
    zpShaderManager m_shaderManager;
    zpMaterialManager m_materialManager;
    zpFontManager m_fontManager;
    zpMeshManager m_meshManager;
    zpCameraManager m_cameraManager;

    zpTime m_time;

    zp_handle m_hWnd;
    zp_handle m_hInstance;

    zp_size_t m_frameCount;
    zp_uint m_flags;
    zp_uint m_debugFlags;

    zpVector2i m_screenSize;

    zp_int m_targetFps;
    zp_int m_targetFixedFps;

    zpApplicationExitCode m_exitCode;

    zp_bool m_isRunning;
    zp_bool m_isPaused;
    zp_bool m_isFocused;
    zp_bool m_shouldRestart;
    zp_bool m_shouldGarbageCollect;
    zp_bool m_shouldReloadAllResources;
    zp_bool m_shouldPauseInBackground;
};

#endif // !ZP_BASE_APPLICATION_H
