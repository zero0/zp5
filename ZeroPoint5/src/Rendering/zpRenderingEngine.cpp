#include "Rendering/zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

enum zpClearPassInput
{
    ZP_CLEAR_PASS_INPUT_COLOR_RT,
    ZP_CLEAR_PASS_INPUT_DEPTH_RT,

    zpClearPassInput_Count,
};

enum zpClearPassOutput
{
    ZP_CLEAR_PASS_OUTPUT_COLOR_RT,
    ZP_CLEAR_PASS_OUTPUT_DEPTH_RT,

    zpClearPassOutput_Count,
};

class zpClearPass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void setInput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_inputs[ index ] = renderResource;
    }

    void setOutput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_outputs[ index ] = renderResource;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        zpRenderCommandBuffer* commandBuffer = renderContext->getCommandBuffer();

        commandBuffer->pushMarker( "Clear" );

        // set rt
        zpRenderTargetIdentifier rtColor = { &m_inputs[ ZP_CLEAR_PASS_INPUT_COLOR_RT ].getTexture() };
        zpRenderTargetIdentifier rtDepth = { &m_inputs[ ZP_CLEAR_PASS_INPUT_DEPTH_RT ].getTexture() };

        commandBuffer->setRenderTarget( rtColor, rtDepth );

        // set viewport
        commandBuffer->setViewport( camera->viewport );
        commandBuffer->setScissorRect( camera->clipRect );

        // clear camera
        if( camera->clearMode )
        {
            if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
            {
                commandBuffer->clearColorDepthStencil( camera->clearColor, camera->clearDepth, camera->clearStencil );
            }
            else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH ) )
            {
                commandBuffer->clearColorDepth( camera->clearColor, camera->clearDepth );
            }
            else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
            {
                commandBuffer->clearDepthStencil( camera->clearDepth, camera->clearStencil );
            }
            else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR ) )
            {
                commandBuffer->clearColor( camera->clearColor );
            }
            else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_DEPTH ) )
            {
                commandBuffer->clearDepth( camera->clearDepth );
            }
            else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_STENCIL ) )
            {
                commandBuffer->clearStencil( camera->clearStencil );
            }
        }

        commandBuffer->popMarker();
    };

private:
    zpRenderResourceIdentifier m_inputs[ zpClearPassInput_Count ];
    zpRenderResourceIdentifier m_outputs[ zpClearPassOutput_Count ];
};

enum zpForwardOpaquePassInput
{
    ZP_FORWARD_OPAQUE_PASS_INPUT_COLOR_RT,
    ZP_FORWARD_OPAQUE_PASS_INPUT_DEPTH_RT,

    zpForwardOpaquePassInput_Count
};

enum zpForwardOpaquePassOutput
{
    ZP_FORWARD_OPAQUE_PASS_OUTPUT_COLOR_RT,
    ZP_FORWARD_OPAQUE_PASS_OUTPUT_DEPTH_RT,

    zpForwardOpaquePassOutput_Count
};

class zpForwardOpaquePass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void setInput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_inputs[ index ] = renderResource;
    }

    void setOutput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_outputs[ index ] = renderResource;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        zpRenderCommandBuffer* commandBuffer = renderContext->getCommandBuffer();
        
        commandBuffer->pushMarker( "Forward Opaque" );

        // set rt
        zpRenderTargetIdentifier rtColor = { &m_inputs[ ZP_CLEAR_PASS_INPUT_COLOR_RT ].getTexture() };
        zpRenderTargetIdentifier rtDepth = { &m_inputs[ ZP_CLEAR_PASS_INPUT_DEPTH_RT ].getTexture() };

        commandBuffer->setRenderTarget( rtColor, rtDepth );

        // draw opaque renderers
        zpDrawRenderersDesc desc;
        desc.sortOrder = ZP_RENDER_SORT_ORDER_FRONT_TO_BACK;
        desc.renderRange.minRenderQueue = ZP_RENDER_QUEUE_OPAQUE;
        desc.renderRange.maxRenderQueue = ZP_RENDER_QUEUE_TRANSPARENT;
        desc.renderLayers = -1;
        desc.passIndex = 0;
        desc.viewPorts = -1;

        renderContext->drawRenderers( desc );

        commandBuffer->popMarker();
    };

private:
    zpRenderResourceIdentifier m_inputs[ zpForwardOpaquePassInput_Count ];
    zpRenderResourceIdentifier m_outputs[ zpForwardOpaquePassOutput_Count ];
};

enum zpForwardTransparnetPassInput
{
    ZP_FORWARD_TRANSPARENT_PASS_INPUT_COLOR_RT,
    ZP_FORWARD_TRANSPARENT_PASS_INPUT_DEPTH_RT,

    zpForwardTransparentPassInput_Count
};

enum zpForwardTransparentPassOutput
{
    ZP_FORWARD_TRANSPARENT_PASS_OUTPUT_COLOR_RT,
    ZP_FORWARD_TRANSPARENT_PASS_OUTPUT_DEPTH_RT,

    zpForwardTransparentPassOutput_Count
};

class zpForwardTransparentPass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void setInput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_inputs[ index ] = renderResource;
    }

    void setOutput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_outputs[ index ] = renderResource;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        zpRenderCommandBuffer* commandBuffer = renderContext->getCommandBuffer();

        commandBuffer->pushMarker( "Forward Transparent" );

        // set rt
        zpRenderTargetIdentifier rtColor = { &m_inputs[ ZP_FORWARD_TRANSPARENT_PASS_INPUT_COLOR_RT ].getTexture() };
        zpRenderTargetIdentifier rtDepth = { &m_inputs[ ZP_FORWARD_TRANSPARENT_PASS_INPUT_DEPTH_RT ].getTexture() };

        commandBuffer->setRenderTarget( rtColor, rtDepth );

        // draw opaque renderers
        zpDrawRenderersDesc desc;
        desc.sortOrder = ZP_RENDER_SORT_ORDER_BACK_TO_FRONT;
        desc.renderRange.minRenderQueue = ZP_RENDER_QUEUE_TRANSPARENT;
        desc.renderRange.maxRenderQueue = ZP_RENDER_QUEUE_MAX;
        desc.renderLayers = -1;
        desc.passIndex = 0;
        desc.viewPorts = -1;

        renderContext->drawRenderers( desc );

        commandBuffer->popMarker();
    };

private:
    zpRenderResourceIdentifier m_inputs[ zpForwardTransparentPassInput_Count ];
    zpRenderResourceIdentifier m_outputs[ zpForwardTransparentPassOutput_Count ];
};

enum zpResolveAntiAliasPassInput
{
    ZP_RESOLVE_ANTI_ALIAS_PASS_INPUT_RT,

    zpResolveAntiAliasPassInput_Count
};

enum zpResolveAntiAliasPassOutput
{
    ZP_RESOLVE_ANTI_ALIAS_PASS_OUTPUT_RT,

    zpResolveAntiAliasPassOutput_Count
};

class zpResolveAntiAliasPass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void setInput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_inputs[ index ] = renderResource;
    }

    void setOutput( zp_size_t index, const zpRenderResourceIdentifier& renderResource )
    {
        m_outputs[ index ] = renderResource;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        zpRenderCommandBuffer* commandBuffer = renderContext->getCommandBuffer();

        commandBuffer->pushMarker( "Resolve AA" );

        // set rt
        zpRenderTargetIdentifier rtSrc = { &m_inputs[ ZP_RESOLVE_ANTI_ALIAS_PASS_INPUT_RT ].getTexture() };
        zpRenderTargetIdentifier rtDst = { ZP_NULL }; //&m_outputs[ ZP_RESOLVE_ANTI_ALIAS_PASS_OUTPUT_RT ].getTexture() };

        commandBuffer->resolveAntiAlias( rtSrc, rtDst );

        commandBuffer->popMarker();
    };

private:
    zpRenderResourceIdentifier m_inputs[ zpResolveAntiAliasPassInput_Count ];
    zpRenderResourceIdentifier m_outputs[ zpResolveAntiAliasPassOutput_Count ];
};

//
//
//

zpRenderingEngine::zpRenderingEngine()
{

}
zpRenderingEngine::~zpRenderingEngine()
{

}

void zpRenderingEngine::setup( zp_handle hWindow )
{
    ZP_PROFILER_BLOCK();

    //m_engine.setup( hWindow );
    SetupRenderingOpenGL( hWindow, m_hDC, m_hContext );

    m_immediate.setup( this, &m_context );
    m_resourceCache.setup( this );

    zpTexture mainColorMsaa;
    zpTexture mainDepthMsaa;

    zpCreateTextureDesc desc;
    desc.textureName = "MainColorMSAA";
    desc.pixels = ZP_NULL;
    desc.desc.width = 960;
    desc.desc.height = 640;
    desc.desc.depth = 0;
    desc.desc.mipMapCount = 1;
    desc.desc.minMipMap = 0;
    desc.desc.multisampleCount = 4;
    desc.desc.lodBias = 0;
    desc.desc.format = ZP_DISPLAY_FORMAT_RGBA16_UNORM;
    desc.desc.textureDimension = ZP_TEXTURE_DIMENSION_2D_MULTISAMPLE;
    desc.desc.type = ZP_TEXTURE_TYPE_TEXTURE;

    createTexture( &desc, mainColorMsaa );

    desc.textureName = "MainDepthMSAA";
    desc.desc.format = ZP_DISPLAY_FORMAT_D24S8_UNORM_UINT;

    createTexture( &desc, mainDepthMsaa );

    zpRenderResourceIdentifier colorMsaa;
    zpRenderResourceIdentifier depthMsaa;

    m_resourceCache.createTexture( "mainColorMsaa", mainColorMsaa, colorMsaa );
    m_resourceCache.createTexture( "mainDepthMsaa", mainDepthMsaa, depthMsaa );

    zpClearPass* clearPass = m_pipeline.addPass<zpClearPass>();
    clearPass->setInput( ZP_CLEAR_PASS_INPUT_COLOR_RT, colorMsaa );
    clearPass->setInput( ZP_CLEAR_PASS_INPUT_DEPTH_RT, depthMsaa );

    zpForwardOpaquePass* fwdOpaquePass = m_pipeline.addPass<zpForwardOpaquePass>();
    fwdOpaquePass->setInput( ZP_FORWARD_OPAQUE_PASS_INPUT_COLOR_RT, colorMsaa );
    fwdOpaquePass->setInput( ZP_FORWARD_OPAQUE_PASS_INPUT_DEPTH_RT, depthMsaa );

    zpForwardTransparentPass* fwdTransparentPass = m_pipeline.addPass<zpForwardTransparentPass>();
    fwdTransparentPass->setInput( ZP_FORWARD_TRANSPARENT_PASS_INPUT_COLOR_RT, colorMsaa );
    fwdTransparentPass->setInput( ZP_FORWARD_TRANSPARENT_PASS_INPUT_DEPTH_RT, depthMsaa );

    zpResolveAntiAliasPass* resolvePass = m_pipeline.addPass<zpResolveAntiAliasPass>();
    resolvePass->setInput( ZP_RESOLVE_ANTI_ALIAS_PASS_INPUT_RT, colorMsaa );
    //resolvePass->setOutput( ZP_RESOLVE_ANTI_ALIAS_PASS_OUTPUT_RT, backbuffer );

    m_pipeline.rebuild();
}
void zpRenderingEngine::teardown()
{
    ZP_PROFILER_BLOCK();
    
    m_immediate.teardown();
    m_resourceCache.teardown();

    m_pipeline.clearAllPasses();

    m_context.clearDrawRenderables();

    //m_engine.teardown();
    TeardownRenderingOpenGL( m_hContext );

    m_hDC = ZP_NULL;
    m_hContext = ZP_NULL;
}

void zpRenderingEngine::setWindowSize( const zpVector2i& size )
{
    //m_engine.setWindowSize( size );
}

void zpRenderingEngine::beginFrame( zp_size_t frameIndex )
{
    ZP_PROFILER_BLOCK();

    m_currentFrame = frameIndex;

    BeginFrameOpenGL( m_currentFrame );

    m_context.clearDrawRenderables();
    m_context.getCommandBuffer()->reset();
}

void zpRenderingEngine::endFrame()
{
    ZP_PROFILER_BLOCK();

    zpRenderingStat stat;
    EndFrameOpenGL( m_currentFrame, stat );

    ZP_PROFILER_GPU( stat.frameTime, stat.primitiveCount );

    submitCommandBuffer( m_context.getCommandBuffer() );

    m_immediate.flipBuffers();
}

void zpRenderingEngine::submit()
{
    ZP_PROFILER_BLOCK();

    //PresentOpenGL( m_hDC, m_hContext );
    
    zpRenderCommandPresent cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_PRESENT;
    cmd.header.id = -1;

    cmd.hDC = m_hDC;
    cmd.hContext = m_hContext;
    cmd.frameIndex = m_currentFrame;

    submitRawCommand( &cmd, sizeof( zpRenderCommandPresent ) );
}

zp_bool zpRenderingEngine::isVSync() const
{
    return m_isVSync;
}
void zpRenderingEngine::setVSync( zp_bool vsync )
{
    m_isVSync = vsync;
}

zpRenderImmediate* zpRenderingEngine::getImmediate()
{
    return &m_immediate;
}

zpRenderContext* zpRenderingEngine::getContext()
{
    return &m_context;
}

zpRenderPipeline* zpRenderingEngine::getPipeline()
{
    return &m_pipeline;
}

zpRenderResourceCache* zpRenderingEngine::getRenderResourceCache()
{
    return &m_resourceCache;
}

void zpRenderingEngine::createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer )
{
    ZP_PROFILER_BLOCK();
    
    CreateRenderBufferOpenGL( type, bindType, size, data, buffer );
}

void zpRenderingEngine::destoryRenderBuffer( zpRenderBuffer& buffer )
{
    ZP_PROFILER_BLOCK();

    DestroyRenderBufferOpenGL( buffer );
}

void zpRenderingEngine::createTexture( zpCreateTextureDesc* desc, zpTexture& texture )
{
    ZP_PROFILER_BLOCK();
    if( desc )
    {
        CreateTextureOpenGL( desc, texture );
    }
}

void zpRenderingEngine::destroyTexture( zpTexture& texture )
{
    ZP_PROFILER_BLOCK();

    DestroyTextureOpenGL( texture );
}

void zpRenderingEngine::createShader( zpCreateShaderDesc* desc, zpShader& shader )
{
    ZP_PROFILER_BLOCK();
    if( desc )
    {
        CreateShaderOpenGL( desc, shader );
    }
}

void zpRenderingEngine::destroyShader( zpShader& shader )
{
    ZP_PROFILER_BLOCK();
   
    DestroyShaderOpenGL( shader );
}


void zpRenderingEngine::submitCommandBuffer( zpRenderCommandBuffer* buffer )
{
    if( buffer && buffer->getLength() )
    {
        ProcessRenderCommandOpenGL( buffer->getBuffer(), buffer->getLength() );
    }
}

void zpRenderingEngine::submitRawCommand( const void* cmd, zp_size_t size )
{
    ProcessRenderCommandOpenGL( cmd, size );
}
