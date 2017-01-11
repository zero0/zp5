#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

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

    zpRenderingContext* getImmidiateContext();

private:
    //zpRenderingEngineImpl m_engine;
    zpRenderingContext m_immidiateContext;

    zp_handle m_hDC;
    zp_handle m_hContext;

    zp_bool m_isVSync;
};

#endif // !ZP_RENDERING_ENGINE_H
