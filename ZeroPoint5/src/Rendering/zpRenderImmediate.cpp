#include "Rendering/zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

static ZP_FORCE_INLINE zpIndexStride _IndexCountToStride( zp_size_t indexCount )
{
    return indexCount <= 0xFF ? ZP_INDEX_STRIDE_BYTE : indexCount <= 0xFFFF ? ZP_INDEX_STRIDE_USHORT : ZP_INDEX_STRIDE_UINT;
}

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
        desc.indexStride = _IndexCountToStride( m_currentIndexCount );
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

    m_currentMaterial.release();
}

void zpRenderImmediate::beginDrawText( const zpFontHandle& font )
{
    beginDrawImmediate( ZP_TOPOLOGY_TRIANGLE_LIST, ZP_VERTEX_FORMAT_VERTEX_COLOR_UV );

    m_currentFont = font;
    m_currentMaterial = font->fontMaterial;
}

zp_uint zpRenderImmediate::addText( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor )
{
    return 0;
}

zp_uint zpRenderImmediate::addTextShadow( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor, const zpVector4f& shadowOffset, const zpColor32i& shadowColor )
{
    zp_char prev = '\0';
    zp_char curr = '\0';

    const zpFont* font = m_currentFont.get();

    const zp_float scale = static_cast<zp_float>( size ) / static_cast<zp_float>( font->baseFontSize );
    const zp_uint lineHeight = static_cast<zp_uint>( ( static_cast<zp_float>( size ) * scale ) + 1.f ) + lineSpacing;

    zpScalarSimd sScale = zpMath::Scalar( scale );

    ZP_ALIGN16 zpVector4f p = pos;
    zpVector4fSimd cursor = zpMath::Vector4Load4( p.m );
    zpVector4fSimd origin = cursor;
    zpVector4fSimd newLine = zpMath::Vector4( 0, static_cast<zp_float>( lineHeight ), 0, 0 );
    zp_uint finalLineHeight = lineHeight;

    p = shadowOffset;
    zpVector4fSimd shadow = zpMath::Vector4Load4( p.m );

    ZP_ALIGN16 zpVector4f p0, p1, p2, p3;

    zp_ushort baseIndexOffset = static_cast<zp_ushort>( m_currentVertexCount );

    for( ; ( curr = *text ); ++text )
    {
        switch( curr )
        {
            case '\n':
                finalLineHeight += lineHeight;
                cursor = zpMath::Vector4Add( cursor, newLine );
                cursor = zpMath::Vector4SetX( cursor, zpMath::Vector4GetX( origin ) );
                continue;

            case '\r':
                continue;
        }

        const zpGlyph* g = font->fontGlyphs + curr;

        zpVector4fSimd tl, tr, br, bl;
        zpVector4fSimd stl, str, sbr, sbl;

        zp_float minX = static_cast<zp_float>( g->bearing.x );
        zp_float maxX = static_cast<zp_float>( g->bearing.x + g->size.x );
        zp_float minY = static_cast<zp_float>( -( g->size.y - g->bearing.y ) );
        zp_float maxY = static_cast<zp_float>( g->bearing.y );

        bl = zpMath::Vector4Scale( zpMath::Vector4( minX, minY, 0, 0 ), sScale );
        tl = zpMath::Vector4Scale( zpMath::Vector4( minX, maxY, 0, 0 ), sScale );
        tr = zpMath::Vector4Scale( zpMath::Vector4( maxX, maxY, 0, 0 ), sScale );
        br = zpMath::Vector4Scale( zpMath::Vector4( maxX, minY, 0, 0 ), sScale );

        bl = zpMath::Vector4Add( cursor, bl );
        tl = zpMath::Vector4Add( cursor, tl );
        tr = zpMath::Vector4Add( cursor, tr );
        br = zpMath::Vector4Add( cursor, br );

        cursor = zpMath::Vector4Add( cursor, zpMath::Vector4Scale( zpMath::Vector4( static_cast<zp_float>( g->advance ), 0, 0, 0 ), sScale ) );

        zpVector2f u0 = { g->uvRect.x,                   g->uvRect.y + g->uvRect.height };
        zpVector2f u1 = { g->uvRect.x,                   g->uvRect.y };
        zpVector2f u2 = { g->uvRect.x + g->uvRect.width, g->uvRect.y };
        zpVector2f u3 = { g->uvRect.x + g->uvRect.width, g->uvRect.y + g->uvRect.height };

        // add shadow geo
        sbl = zpMath::Vector4Add( bl, shadow );
        stl = zpMath::Vector4Add( tl, shadow );
        str = zpMath::Vector4Add( tr, shadow );
        sbr = zpMath::Vector4Add( br, shadow );

        zpMath::Vector4Store4( sbl, p0.m );
        zpMath::Vector4Store4( stl, p1.m );
        zpMath::Vector4Store4( str, p2.m );
        zpMath::Vector4Store4( sbr, p3.m );

        // bl -> tl -> tr -> br
        addVertex( p0, shadowColor, u0 );
        addVertex( p1, shadowColor, u1 );
        addVertex( p2, shadowColor, u2 );
        addVertex( p3, shadowColor, u3 );

        addQuadIndex(
            baseIndexOffset + 0,
            baseIndexOffset + 1,
            baseIndexOffset + 2,
            baseIndexOffset + 3 );

        baseIndexOffset += 4;

        // add normal geo
        zpMath::Vector4Store4( bl, p0.m );
        zpMath::Vector4Store4( tl, p1.m );
        zpMath::Vector4Store4( tr, p2.m );
        zpMath::Vector4Store4( br, p3.m );

        // bl -> tl -> tr -> br
        addVertex( p0, bottomColor, u0 );
        addVertex( p1, topColor, u1 );
        addVertex( p2, topColor, u2 );
        addVertex( p3, bottomColor, u3 );

        addQuadIndex(
            baseIndexOffset + 0,
            baseIndexOffset + 1,
            baseIndexOffset + 2,
            baseIndexOffset + 3 );

        baseIndexOffset += 4;

        prev = curr;
    }

    return finalLineHeight;
}

void zpRenderImmediate::endDrawText()
{
    m_currentFont.release();

    endDrawImmediate();
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
