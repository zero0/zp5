#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

#ifdef ZP_USE_OPENGL_RENDERING
typedef zpRenderingEngineOpenGL zpRenderingEngineImpl;
#endif

#ifdef ZP_USE_D3D_RENDERING
typedef zpRenderingEngineOpenGL zpRenderingEngineImpl;
#endif

class zpRenderingEngine
{
public:
    zpRenderingEngine();
    ~zpRenderingEngine();

    void setup( zp_handle hWindow );
    void teardown();

    void setWindowSize( const zpVector2i& size );

    void present();

    zp_bool isVSync() const;
    void setVSync( zp_bool vsync );

private:
    zpRenderingEngineImpl m_engine;

    zp_bool m_isVSync;
};

#endif // !ZP_RENDERING_ENGINE_H
