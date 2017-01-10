#include "zpRendering.h"

zpRenderingEngine::zpRenderingEngine()
{

}
zpRenderingEngine::~zpRenderingEngine()
{

}

void zpRenderingEngine::setup( zp_handle hWindow )
{
    m_engine.setup( hWindow );
}
void zpRenderingEngine::teardown()
{
    m_engine.teardown();
}

void zpRenderingEngine::setWindowSize( const zpVector2i& size )
{
    m_engine.setWindowSize( size );
}

void zpRenderingEngine::present()
{
    m_engine.present();
}

zp_bool zpRenderingEngine::isVSync() const
{
    return m_isVSync;
}
void zpRenderingEngine::setVSync( zp_bool vsync )
{
    m_isVSync = vsync;
}
