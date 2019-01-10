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

    const zpVector4fSimd cCenter = zpMath::Vector4Load4( m_currentCamera->position.m );
    const zpScalarSimd invFarDistance = zpMath::ScalarPow2( zpMath::ScalarRcp( zpMath::Scalar( m_currentCamera->zFar ) ) );
    const zpScalarSimd maxDepth = zpMath::Scalar( 65535.f );

    // filter draw commands
    zpVector< zpDrawRenderable >::const_iterator b = m_allRenderables.begin();
    zpVector< zpDrawRenderable >::const_iterator e = m_allRenderables.end();
    for( ; b != e; ++b )
    {
        zpVector< zpDrawRenderable >::const_reference p = *b;

        const zp_bool isPassMatch = p.passIndex == desc.passIndex;
        const zp_bool isRenderLayer = ( p.renderLayers & desc.renderLayers ) != 0;
        const zp_bool isInRenderRange = p.material->renderQueue >= desc.renderRange.minRenderQueue && p.material->renderQueue < desc.renderRange.maxRenderQueue;
        const zp_bool isInViewport = true; // TODO: implement viewports
        if( isPassMatch && isRenderLayer && isInRenderRange && isInViewport )
        {
            zp_int depth = 0;
            if( m_currentCamera->projectionType >= ZP_CAMERA_PROJECTION_PERSPECTIVE )
            {
                const zpVector4fSimd pCenter = zpMath::Vector4Load4( p.bounds.center.m );
                zpScalarSimd dist = zpMath::Vector4LengthSquared3( zpMath::Vector4Sub( pCenter, cCenter ) );
                zpScalarSimd linearDist = zpMath::ScalarMul( dist, invFarDistance );
                //depth = zpMath::ScalarFloorToInt( zpMath::ScalarMul( linearDist, maxDepth ) );
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
    zp_qsort3( m_renderableSort.begin(), m_renderableSort.end(), [ &desc ]( const zpDrawRenderableSort& a, const zpDrawRenderableSort& b )
    {
        zp_int cmp = 0;
        if( desc.sortOrder == ZP_RENDER_SORT_ORDER_BACK_TO_FRONT )
        {
            cmp = b.sortKey.depth - a.sortKey.depth;
        }
        else
        {
            cmp = a.sortKey.depth - b.sortKey.depth;
        }

        if( cmp == 0 )
        {
            cmp = a.sortKey.layer < b.sortKey.layer ? -1 : a.sortKey.layer > b.sortKey.layer ? 1 : 0;
        }

        if( cmp == 0 )
        {
            cmp = a.sortKey.material < b.sortKey.material ? -1 : a.sortKey.material > b.sortKey.material ? 1 : 0;
        }

        if( cmp == 0 )
        {
            cmp = a.sortKey.pass < b.sortKey.pass ? -1 : a.sortKey.pass > b.sortKey.pass ? 1 : 0;
        }

        return cmp;
    } );

    // add render commands to command buffer in sort order
    zpVector< zpDrawRenderableSort >::const_iterator sb = m_renderableSort.begin();
    zpVector< zpDrawRenderableSort >::const_iterator se = m_renderableSort.end();
    for( ; sb != se; ++sb )
    {
        const zpDrawRenderable* drawRenderable = sb->drawRenderable;
        if( drawRenderable == ZP_NULL ) continue;

        if( drawRenderable->mesh.isValid() )
        {
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

            zpDrawMeshDesc desc;
            for( ; startSubMesh < endSubMesh; ++startSubMesh )
            {
                const zpMesh* mesh = drawRenderable->mesh.get();
                const zpMeshPart& part = mesh->parts[ startSubMesh ];

                desc.vertexBuffer = mesh->vertexData;
                desc.indexBuffer = mesh->indexData;
                desc.vertexOffset = part.vertexOffset;
                desc.vertexCount = part.vertexCount;
                desc.indexOffset = part.indexOffset;
                desc.indexCount = part.indexCount;
                desc.indexStride = mesh->indexStride;
                desc.vertexFormat = mesh->vertexFormat;
                desc.topology = mesh->topology;

                m_commandBuffer.drawMesh( drawRenderable->localToWorld, desc, drawRenderable->material.get(), drawRenderable->passIndex );
            }
        }
        else
        {
            zpDrawMeshDesc desc;
            desc.vertexBuffer = drawRenderable->vertexBuffer;
            desc.indexBuffer = drawRenderable->indexBuffer;
            desc.vertexOffset = drawRenderable->vertexOffset;
            desc.vertexCount = drawRenderable->vertexCount;
            desc.indexOffset = drawRenderable->indexOffset;
            desc.indexCount = drawRenderable->indexCount;
            desc.indexStride = drawRenderable->indexStride;
            desc.vertexFormat = drawRenderable->vertexFormat;
            desc.topology = drawRenderable->topology;

            m_commandBuffer.drawMesh( drawRenderable->localToWorld, desc, drawRenderable->material.get(), drawRenderable->passIndex );
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
}

const zpCamera* zpRenderContext::getActiveCamera() const
{
    return m_currentCamera;
}

zpRenderCommandBuffer* zpRenderContext::getCommandBuffer()
{
    return &m_commandBuffer;
}

void zpRenderContext::submit()
{

}
