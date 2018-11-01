#include "Rendering/zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

zpRenderingEngine::zpRenderingEngine()
{

}
zpRenderingEngine::~zpRenderingEngine()
{

}

class zpClearPass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        zpRenderCommandBuffer* commandBuffer = renderContext->getCommandBuffer();

        commandBuffer->pushMarker( "Clear" );

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
};

class zpForwardOpaquePass : public zpRenderPipelinePass
{
public:
    zpRenderPipelinePassResolveResult resolve()
    {
        markAsClean();
        return ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS;
    }

    void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
        renderContext->getCommandBuffer()->pushMarker( "Forward Opaque" );

        zpDrawRenderersDesc desc;
        desc.sortOrder = ZP_RENDER_SORT_ORDER_FRONT_TO_BACK;
        desc.renderRange.minRenderQueue = 0;
        desc.renderRange.maxRenderQueue = 2000;
        desc.renderLayers = -1;
        desc.passIndex = 0;
        desc.viewPorts = -1;

        renderContext->drawRenderers( desc );

        renderContext->getCommandBuffer()->popMarker();
    };
};

void zpRenderingEngine::setup( zp_handle hWindow )
{
    ZP_PROFILER_BLOCK();

    //m_engine.setup( hWindow );
    SetupRenderingOpenGL( hWindow, m_hDC, m_hContext );

    m_immediate.setup( this, &m_context );

    m_pipeline.addPass<zpClearPass>();
    m_pipeline.addPass<zpForwardOpaquePass>();

    m_pipeline.rebuild();
}
void zpRenderingEngine::teardown()
{
    ZP_PROFILER_BLOCK();
    
    m_immediate.teardown();

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
