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

    m_immidiateContext.setup();
}
void zpRenderingEngine::teardown()
{
    m_immidiateContext.teardown();

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

zpRenderingContext* zpRenderingEngine::getImmidiateContext()
{
    return &m_immidiateContext;
}

void zpRenderingEngine::createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer )
{
    CreateRenderBufferOpenGL( type, bindType, size, data, buffer );
}

void zpRenderingEngine::destoryRenderBuffer( zpRenderBuffer& buffer )
{
    DestroyRenderBufferOpenGL( buffer );
}

void zpRenderingEngine::createTexture( zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture )
{
    CreateTextureOpenGL( width, height, mipMapCount, displayFormat, textureDimension, textureType, pixels, texture );
}

void zpRenderingEngine::destoryTexture( zpTexture& texture )
{
    DestroyTextureOpenGL( texture );
}
