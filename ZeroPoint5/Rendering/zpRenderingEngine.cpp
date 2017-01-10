#include "zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

zpRenderingEngine::zpRenderingEngine()
{

}
zpRenderingEngine::~zpRenderingEngine()
{

}

void zpRenderingEngine::setup( zp_handle hWindow )
{
    //m_engine.setup( hWindow );
    SetupRenderingOpenGL( hWindow, m_hDC, m_hContext );
}
void zpRenderingEngine::teardown()
{
    //m_engine.teardown();
    TeardownRenderingOpenGL( m_hContext );

    m_hDC = ZP_NULL;
    m_hContext = ZP_NULL;
}

void zpRenderingEngine::setWindowSize( const zpVector2i& size )
{
    //m_engine.setWindowSize( size );
}

void zpRenderingEngine::present()
{
    m_immidiateContext.fillBuffers();
    m_immidiateContext.processCommands( ProcessRenderingCommandOpenGL );
    m_immidiateContext.flipBuffers();

    //m_engine.present();
    PresentOpenGL( m_hDC, m_hContext );
}

zp_bool zpRenderingEngine::isVSync() const
{
    return m_isVSync;
}
void zpRenderingEngine::setVSync( zp_bool vsync )
{
    m_isVSync = vsync;
}
