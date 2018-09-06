#include "Rendering/zpRendering.h"

zpRenderContext::zpRenderContext()
    : m_currentCamera( ZP_NULL )
    , m_commandBuffer( 1024 * 2 )
    , m_allRenderables( 128 )
    , m_renderableSort( 128 )
{
}

zpRenderContext::~zpRenderContext()
{
}

void zpRenderContext::addDrawRenderable( const zpDrawRenderable& renderable )
{
    m_allRenderables.pushBack( renderable );
}

void zpRenderContext::clearDrawRenderables()
{
    m_allRenderables.clear();

    m_renderableSort.reset();
}

void zpRenderContext::drawRenderers( const zpDrawRenderersDesc& desc )
{
    ZP_PROFILER_BLOCK();
   
    m_renderableSort.reset();

    m_renderableSort.reserve( m_allRenderables.size() );

    const zpVector4f cCenter = zpMath::Vector4Load4( m_currentCamera->position.m );
    const zpScalar invFarDistance = zpMath::ScalarRcp( zpMath::Scalar( m_currentCamera->zFar ) );

    // filter draw commands
    zpVector< zpDrawRenderable >::const_iterator b = m_allRenderables.begin();
    zpVector< zpDrawRenderable >::const_iterator e = m_allRenderables.end();
    for( ; b != e; ++b )
    {
        zpVector< zpDrawRenderable >::const_reference p = *b;

        const zp_bool isPassMatch = p.passIndex == desc.passIndex;
        const zp_bool isRenderLayer = ( p.renderLayers & desc.renderLayers ) != 0;
        const zp_bool isInRenderRange = true; // TODO: implement render queue range in zpMaterial
        const zp_bool isInViewport = true; // TODO: implement viewports
        if( isPassMatch && isRenderLayer && isInRenderRange && isInViewport )
        {
            zp_uint depth = 0;
            if( m_currentCamera->projectionType >= ZP_CAMERA_PROJECTION_PERSPECTIVE )
            {
                const zpVector4f pCenter = zpMath::Vector4Load4( p.bounds.center.m );
                zpScalar dist = zpMath::Vector4LengthSquared3( zpMath::Vector4Sub( pCenter, cCenter ) );
                dist = zpMath::ScalarMul( dist, invFarDistance );
                depth = zp_floor_to_int( zpMath::AsFloat( dist ) * 65535.0f );
            }

            zpDrawRenderableSort& sort = m_renderableSort.pushBackEmpty();
            sort.drawRenderable = b;
            sort.sortKey.viewport = 0;
            sort.sortKey.layer = p.renderLayers;
            sort.sortKey.depth = depth;
            sort.sortKey.material = 0;
            sort.sortKey.pass = p.passIndex;
        }
    }

    // sort draw commands
    // TODO: implement full sorting
    zp_qsort( m_renderableSort.begin(), m_renderableSort.end(), []( const zpDrawRenderableSort& a, const zpDrawRenderableSort& b )
    {
        return a.sortKey.layer < b.sortKey.layer;
    } );

    // add render commands to command buffer in sort order
    zpVector< zpDrawRenderableSort >::const_iterator sb = m_renderableSort.begin();
    zpVector< zpDrawRenderableSort >::const_iterator se = m_renderableSort.end();
    for( ; sb != se; ++sb )
    {
        const zpDrawRenderable* drawRenderable = sb->drawRenderable;

        zp_size_t startSubMesh;
        zp_size_t endSubMesh;
        if( drawRenderable->subMeshIndex < 0 )
        {
            startSubMesh = 0;
            endSubMesh = drawRenderable->mesh->numMeshParts;
        }
        else
        {
            startSubMesh = drawRenderable->subMeshIndex;
            endSubMesh = startSubMesh + 1;
        }

        for( ; startSubMesh < endSubMesh; ++startSubMesh )
        {
            m_commandBuffer.drawMesh( drawRenderable->localToWorld, drawRenderable->mesh.get(), drawRenderable->material.operator->(), startSubMesh, drawRenderable->passIndex );
        }
    }
}

void zpRenderContext::executeCommandBuffer( const zpRenderCommandBuffer& buffer )
{
    ZP_PROFILER_BLOCK();

    m_commandBuffer.executeCommandBuffer( buffer );
}

void zpRenderContext::setActiveCamera( const zpCamera* camera )
{
    ZP_PROFILER_BLOCK();

    m_currentCamera = camera;

    // TODO: move to pass
    // set viewport
    m_commandBuffer.setViewport( camera->viewport );
    m_commandBuffer.setScissorRect( camera->clipRect );

    // clear camera
    if( camera->clearMode )
    {
        if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
        {
            m_commandBuffer.clearColorDepthStencil( camera->clearColor, camera->clearDepth, camera->clearStencil );
        }
        else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR | ZP_CAMERA_CLEAR_MODE_DEPTH ) )
        {
            m_commandBuffer.clearColorDepth( camera->clearColor, camera->clearDepth );
        }
        else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_DEPTH | ZP_CAMERA_CLEAR_MODE_STENCIL ) )
        {
            m_commandBuffer.clearDepthStencil( camera->clearDepth, camera->clearStencil );
        }
        else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_COLOR ) )
        {
            m_commandBuffer.clearColor( camera->clearColor );
        }
        else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_DEPTH ) )
        {
            m_commandBuffer.clearDepth( camera->clearDepth );
        }
        else if( camera->clearMode & ( ZP_CAMERA_CLEAR_MODE_STENCIL ) )
        {
            m_commandBuffer.clearStencil( camera->clearStencil );
        }
    }
}

const zpCamera* zpRenderContext::getActiveCamera() const
{
    return m_currentCamera;
}

void zpRenderContext::submit()
{

}
