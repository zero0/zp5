#pragma once
#ifndef ZP_RENDERING_ENGINE_OPENGL_H
#define ZP_RENDERING_ENGINE_OPENGL_H

class zpRenderingEngineOpenGL
{
public:
    zpRenderingEngineOpenGL();
    ~zpRenderingEngineOpenGL();

    void setup( zp_handle hWindow );
    void teardown();

    void setWindowSize( const zpVector2i& size );

    void present();

private:
    zp_handle m_hWindow;
    zp_handle m_hDC;
    zp_handle m_hContext;
};

#endif // !ZP_RENDERING_ENGINE_OPENGL_H
