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

    void createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer );
    void destoryRenderBuffer( zpRenderBuffer& buffer );

    void createTexture( zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture );
    void destroyTexture( zpTexture& texture );

    void createShader( const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader );
    void destroyShader( zpShader& shader );

private:
    void renderingPassForward( zpRenderingContext& context );

    zpRenderingContext m_immidiateContext;

    zpVector< zpSortRenderCommandData > m_sortedCommands;

    zp_handle m_hDC;
    zp_handle m_hContext;

    zp_bool m_isVSync;
};

#endif // !ZP_RENDERING_ENGINE_H
