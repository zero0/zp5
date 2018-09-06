#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

struct zpCreateRenderBufferDesc
{

};

struct zpCreateTextureDesc
{

};

struct zpCreateShaderDesc
{

};

class zpRenderingEngine
{
public:
    zpRenderingEngine();
    ~zpRenderingEngine();

    void setup( zp_handle hWindow );
    void teardown();

    void setWindowSize( const zpVector2i& size );

    void beginFrame( zp_size_t frameIndex );
    void renderCamera( const zpCamera* camera );
    void endFrame();
    void submit();

    zp_bool isVSync() const;
    void setVSync( zp_bool vsync );

    zpRenderingContext* getImmidiateContext();

    void createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer );
    void destoryRenderBuffer( zpRenderBuffer& buffer );

    void createTexture( const zp_char* textureName, zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture );
    void destroyTexture( zpTexture& texture );

    void createShader( const zp_char* shaderName, const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader );
    void destroyShader( zpShader& shader );

private:
    void renderingPassForward( zpRenderingContext& context );

    void submitCommandBuffer( zpRenderCommandBuffer* buffer );
    void submitRawCommand( const void* cmd, zp_size_t size );

    zpRenderingContext m_immidiateContext;

    zpRenderContext m_context;
    zpRenderPipeline m_pipeline;
    zpRenderCommandBuffer m_commandBuffer;

    zpVector< zpSortRenderCommandData > m_sortedCommands;

    zp_handle m_hDC;
    zp_handle m_hContext;
    zp_size_t m_currentFrame;
    zp_bool m_isVSync;
};

#endif // !ZP_RENDERING_ENGINE_H
