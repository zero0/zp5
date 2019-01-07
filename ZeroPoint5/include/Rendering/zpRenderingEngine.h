#pragma once
#ifndef ZP_RENDERING_ENGINE_H
#define ZP_RENDERING_ENGINE_H

struct zpCreateRenderBufferDesc
{

};

struct zpCreateTextureDesc
{
    const zp_char* textureName;
    const void* pixels;
    zpTextureDesc desc;
};

struct zpCreateShaderDesc
{
    const zp_char* shaderName;

    const void* vertexShaderSrc;
    zp_size_t vertexShaderOffset;
    zp_size_t vertexShaderLength;

    const void* fragmentShaderSrc;
    zp_size_t fragmentShaderOffset;
    zp_size_t fragmentShaderLength;
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
    void endFrame();
    void submit();

    zp_bool isVSync() const;
    void setVSync( zp_bool vsync );

    zpRenderImmediate* getImmediate();
    zpRenderContext* getContext();
    zpRenderPipeline* getPipeline();
    zpRenderResourceCache* getRenderResourceCache();

    void createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer );
    void destoryRenderBuffer( zpRenderBuffer& buffer );

    void createTexture( zpCreateTextureDesc* desc, zpTexture& texture );
    void destroyTexture( zpTexture& texture );

    void createShader( zpCreateShaderDesc* desc, zpShader& shader );
    void destroyShader( zpShader& shader );

private:
    void submitCommandBuffer( zpRenderCommandBuffer* buffer );
    void submitRawCommand( const void* cmd, zp_size_t size );

    zpRenderImmediate m_immediate;
    zpRenderContext m_context;
    zpRenderPipeline m_pipeline;
    zpRenderResourceCache m_resourceCache;

    zpVector< zpSortRenderCommandData > m_sortedCommands;

    zp_handle m_hDC;
    zp_handle m_hContext;
    zp_size_t m_currentFrame;
    zp_bool m_isVSync;
};

#endif // !ZP_RENDERING_ENGINE_H
