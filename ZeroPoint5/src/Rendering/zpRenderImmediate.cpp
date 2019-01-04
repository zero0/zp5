#include "Rendering/zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

zpRenderImmediate::zpRenderImmediate()
    : m_scratchVertexBuffer( ZP_MEMORY_MB( 4 ) )
    , m_scratchIndexBuffer( ZP_MEMORY_MB( 4 ) )
    , m_currentBufferIndex( 0 )
    , m_immediateVertexSize( 0 )
    , m_immediateIndexSize( 0 )
{
}

zpRenderImmediate::~zpRenderImmediate()
{
}

void zpRenderImmediate::setup( zpRenderingEngine* engine, zpRenderContext* ctx )
{
    m_engine = engine;
    m_ctx = ctx;

    m_engine->createRenderBuffer( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 4 ), ZP_NULL, m_immidateVertexBuffers[ 0 ] );
    m_engine->createRenderBuffer( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 4 ), ZP_NULL, m_immidateVertexBuffers[ 1 ] );

    m_engine->createRenderBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateIndexBuffers[ 0 ] );
    m_engine->createRenderBuffer( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateIndexBuffers[ 1 ] );
}

void zpRenderImmediate::teardown()
{
    m_engine->destoryRenderBuffer( m_immidateVertexBuffers[ 0 ] );
    m_engine->destoryRenderBuffer( m_immidateVertexBuffers[ 1 ] );

    m_engine->destoryRenderBuffer( m_immidateIndexBuffers[ 0 ] );
    m_engine->destoryRenderBuffer( m_immidateIndexBuffers[ 1 ] );

    m_engine = ZP_NULL;
    m_ctx = ZP_NULL;
}

void zpRenderImmediate::beginDrawImmediate( zpTopology topology, zpVertexFormat vertexFormat )
{
    m_currentTopology = topology;
    m_currentVertexFormat = vertexFormat;
    m_currentVertexCount = 0;
    m_currentIndexCount = 0;
    
    m_currentFont.release();
    m_currentMaterial.release();

    zpMath::MatrixStore4( zpMath::MatrixIdentity(), m_currentTransform.m );
}

void zpRenderImmediate::beginDrawImmediate( zpMatrix4fParamF transform, zpTopology topology, zpVertexFormat vertexFormat )
{
    m_currentTopology = topology;
    m_currentVertexFormat = vertexFormat;
    m_currentVertexCount = 0;
    m_currentIndexCount = 0;

    m_currentFont.release();
    m_currentMaterial.release();

    zpMath::MatrixStore4( transform, m_currentTransform.m );
}

void zpRenderImmediate::beginDrawImmediate( const zpMatrix4f& transform, zpTopology topology, zpVertexFormat vertexFormat )
{
    m_currentTopology = topology;
    m_currentVertexFormat = vertexFormat;
    m_currentVertexCount = 0;
    m_currentIndexCount = 0;

    m_currentFont.release();
    m_currentMaterial.release();

    m_currentTransform = transform;
}

void zpRenderImmediate::beginDrawImmediate( zpMatrix4fParamF transform, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialHandle& material )
{
    m_currentTopology = topology;
    m_currentVertexFormat = vertexFormat;
    m_currentVertexCount = 0;
    m_currentIndexCount = 0;

    m_currentFont.release();
    m_currentMaterial = material;

    zpMath::MatrixStore4( transform, m_currentTransform.m );
}

void zpRenderImmediate::beginDrawImmediate( const zpMatrix4f& transform, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialHandle& material )
{
    m_currentTopology = topology;
    m_currentVertexFormat = vertexFormat;
    m_currentVertexCount = 0;
    m_currentIndexCount = 0;

    m_currentFont.release();
    m_currentMaterial = material;

    m_currentTransform = transform;
}


void zpRenderImmediate::addVertex( zpVector4fParamF pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentVertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );
    
    ZP_ALIGN16 zpVector4f v;
    zpMath::Vector4Store4( pos, v.m );

    struct
    {
        zpVector4f p;
        zpColor32i c;
    } buff = {
        v,
        color,
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentVertexCount += 1;
}

void zpRenderImmediate::addVertex( zpVector4fParamF pos, const zpColor32i& color, const zpVector2f& uv )
{
    ZP_ASSERT( m_currentVertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

    ZP_ALIGN16 zpVector4f v;
    zpMath::Vector4Store4( pos, v.m );

    struct
    {
        zpVector4f p;
        zpColor32i c;
        zpVector2f u;
    } buff = {
        v,
        color,
        uv
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentVertexCount += 1;
}

void zpRenderImmediate::addVertex( const zpVector4f& pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentVertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    struct
    {
        zpVector4f p;
        zpColor32i c;
    } buff = {
        pos,
        color,
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentVertexCount += 1;
}

void zpRenderImmediate::addVertex( const zpVector4f& pos, const zpColor32i& color, const zpVector2f& uv )
{
    ZP_ASSERT( m_currentVertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

    struct
    {
        zpVector4f p;
        zpColor32i c;
        zpVector2f u;
    } buff = {
        pos,
        color,
        uv
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentVertexCount += 1;
}

void zpRenderImmediate::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
    const zp_ushort buff[] = {
        index0,
        index1,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentIndexCount += 2;
}

void zpRenderImmediate::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
    const zp_ushort buff[] = {
        index0,
        index1,
        index2,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentIndexCount += 3;
}

void zpRenderImmediate::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
{
    const zp_ushort buff[] = {
        index0,
        index1,
        index2,

        index2,
        index3,
        index0,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    m_currentIndexCount += 6;
}

void zpRenderImmediate::setMaterial( const zpMaterialHandle& material )
{
    m_currentMaterial = material;
}

void zpRenderImmediate::setTransform( zpMatrix4fParamF transform )
{
    zpMath::MatrixStore4( transform, m_currentTransform.m );
}

void zpRenderImmediate::setTransform( const zpMatrix4f& transform )
{
    m_currentTransform = transform;
}

void zpRenderImmediate::endDrawImmediate()
{
    if( m_currentVertexCount > 0 && m_currentIndexCount > 0 )
    {
        SetRenderBufferDataOpenGL( m_immidateVertexBuffers[ m_currentBufferIndex ], m_scratchVertexBuffer.getBuffer(), m_immediateVertexSize, m_scratchVertexBuffer.getLength() );
        SetRenderBufferDataOpenGL( m_immidateIndexBuffers[ m_currentBufferIndex ], m_scratchIndexBuffer.getBuffer(), m_immediateIndexSize, m_scratchIndexBuffer.getLength() );

        zpDrawRenderable desc = {};
        desc.renderLayers = -1;
        desc.passIndex = 0;
        desc.subMeshIndex = 0;
        desc.material = m_currentMaterial;
        desc.vertexBuffer = m_immidateVertexBuffers[ m_currentBufferIndex ];
        desc.indexBuffer = m_immidateIndexBuffers[ m_currentBufferIndex ];
        desc.vertexOffset = m_immediateVertexSize;
        desc.vertexCount = m_currentVertexCount;
        desc.indexOffset = m_immediateIndexSize;
        desc.indexCount = m_currentIndexCount;
        desc.indexStride = sizeof( zp_ushort );
        desc.vertexFormat = m_currentVertexFormat;
        desc.topology = m_currentTopology;
        desc.localToWorld = m_currentTransform;
        desc.bounds.center = { 0, 0, 0, 1 };

        m_ctx->addDrawRenderable( desc );

        m_immediateVertexSize += m_scratchVertexBuffer.getLength();
        m_immediateIndexSize += m_scratchIndexBuffer.getLength();

        m_scratchVertexBuffer.clear();
        m_scratchIndexBuffer.clear();
    }

    m_currentVertexCount = 0;
    m_currentIndexCount = 0;
}

void zpRenderImmediate::beginDrawText( const zpFontHandle& font )
{

}

zp_uint zpRenderImmediate::addText( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor )
{
    return 0;
}

zp_uint zpRenderImmediate::addTextShadow( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor, const zpVector4f& shadowOffset, const zpColor32i& shadowColor )
{
    return 0;
}

void zpRenderImmediate::endDrawText()
{

}

zp_size_t zpRenderImmediate::getVertexCount() const
{
    return m_currentVertexCount;
}

zp_size_t zpRenderImmediate::getIndexCount() const
{
    return m_currentIndexCount;
}

void zpRenderImmediate::processCommands( zpRenderCommandBuffer* buffer )
{

}

void zpRenderImmediate::flipBuffers()
{
    m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % 2;

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;

    m_currentVertexCount = 0;
    m_currentIndexCount = 0;
}
