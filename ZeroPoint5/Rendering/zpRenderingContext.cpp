#include "zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

zpRenderingContext::zpRenderingContext()
    : m_currentCommnad( npos )
    , m_commands( 512 )
    , m_scratchVertexBuffer( ZP_MEMORY_MB( 1 ) )
    , m_scratchIndexBuffer( ZP_MEMORY_MB( 1 ) )
    , m_currentBufferIndex( 0 )
    , m_immediateVertexSize( 0 )
    , m_immediateIndexSize( 0 )
{
}

zpRenderingContext::~zpRenderingContext()
{
}

void zpRenderingContext::setup()
{
    CreateRenderBufferOpenGL( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateVertexBuffers[ 0 ] );
    CreateRenderBufferOpenGL( ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_STREAM, ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateVertexBuffers[ 1 ] );

    CreateRenderBufferOpenGL( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_STREAM,  ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateIndexBuffers[ 0 ] );
    CreateRenderBufferOpenGL( ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_STREAM,  ZP_MEMORY_MB( 1 ), ZP_NULL, m_immidateIndexBuffers[ 1 ] );
}

void zpRenderingContext::teardown()
{
    DestroyRenderBufferOpenGL( m_immidateVertexBuffers[ 0 ] );
    DestroyRenderBufferOpenGL( m_immidateVertexBuffers[ 1 ] );

    DestroyRenderBufferOpenGL( m_immidateIndexBuffers[ 0 ] );
    DestroyRenderBufferOpenGL( m_immidateIndexBuffers[ 1 ] );
}

void zpRenderingContext::clear( const zpColorf& clearColor, zp_float clearDepth, zp_uint clearStencil )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_CLEAR;
    cmd.sortKey.key = 0;
    cmd.sortKey.anchor = true;
    cmd.clearColor = clearColor;
    cmd.clearDepth = clearDepth;
    cmd.clearStencil = clearStencil;
}

void zpRenderingContext::flush()
{

}

void zpRenderingContext::setViewport( const zpViewport& viewport )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_SET_VIEWPORT;
    cmd.sortKey.key = 0;
    cmd.sortKey.anchor = true;
    cmd.viewport = viewport;
}

void zpRenderingContext::setScissorRect( const zpRecti& scissorRect )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_SET_SCISSOR_RECT;
    cmd.sortKey.key = 0;
    cmd.sortKey.anchor = true;
    cmd.scissorRect = scissorRect;
}

void zpRenderingContext::resetScissorRect()
{
    zpRecti fullscreen;
    setScissorRect( fullscreen );
}

void zpRenderingContext::beginDrawImmediate( zp_byte layer, zpTopology topology, zpVertexFormat vertexFormat )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );
    m_currentCommnad = m_commands.size();

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
    cmd.sortKey.key = 0;
    cmd.sortKey.layer = layer;
    cmd.topology = topology;
    cmd.vertexFormat = vertexFormat;
    cmd.vertexOffset = 0;
    cmd.vertexCount = 0;
    cmd.indexOffset = 0;
    cmd.indexCount = 0;
    cmd.vertexBuffer = m_immidateVertexBuffers[ m_currentBufferIndex ];
    cmd.indexBuffer = m_immidateIndexBuffers[ m_currentBufferIndex ];
    cmd.material = zpMaterialHandle();
    cmd.font = zpFontHandle();
    cmd.mesh = zpMeshHandle();

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( zpMath::MatrixIdentity(), transformData.m );
    cmd.transform = transformData;
}

void zpRenderingContext::beginDrawImmediate( zp_byte layer, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialHandle& material )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );
    m_currentCommnad = m_commands.size();

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
    cmd.sortKey.key = 0;
    cmd.sortKey.layer = layer;
    cmd.topology = topology;
    cmd.vertexFormat = vertexFormat;
    cmd.vertexOffset = 0;
    cmd.vertexCount = 0;
    cmd.indexOffset = 0;
    cmd.indexCount = 0;
    cmd.vertexBuffer = m_immidateVertexBuffers[ m_currentBufferIndex ];
    cmd.indexBuffer = m_immidateIndexBuffers[ m_currentBufferIndex ];
    cmd.material = material;
    cmd.font = zpFontHandle();
    cmd.mesh = zpMeshHandle();

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( zpMath::MatrixIdentity(), transformData.m );
    cmd.transform = transformData;
}


void zpRenderingContext::beginDrawText( zp_byte layer, const zpFontHandle& font )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );
    m_currentCommnad = m_commands.size();

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
    cmd.sortKey.key = 0;
    cmd.sortKey.layer = layer;
    cmd.topology = ZP_TOPOLOGY_TRIANGLE_LIST;
    cmd.vertexFormat = ZP_VERTEX_FORMAT_VERTEX_COLOR_UV;
    cmd.vertexOffset = 0;
    cmd.vertexCount = 0;
    cmd.indexOffset = 0;
    cmd.indexCount = 0;
    cmd.vertexBuffer = m_immidateVertexBuffers[ m_currentBufferIndex ];
    cmd.indexBuffer = m_immidateIndexBuffers[ m_currentBufferIndex ];
    cmd.material = font->fontMaterial;
    cmd.font = font;
    cmd.mesh = zpMeshHandle();

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( zpMath::MatrixIdentity(), transformData.m );
    cmd.transform = transformData;
}

zp_uint zpRenderingContext::addText( const zpVector4fData& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    zp_char prev = '\0';
    zp_char curr = '\0';

    zpFont* font = cmd->font.get();

    const zp_float scale = static_cast<zp_float>( size ) / static_cast<zp_float>( font->baseFontSize );
    const zp_uint lineHeight = static_cast<zp_uint>( ( static_cast<zp_float>( size ) * scale ) + 1.f );

    zpScalar sScale = zpMath::Scalar( scale );

    ZP_ALIGN16 zpVector4fData p = pos;
    zpVector4f cursor = zpMath::Vector4Load4( p.m );
    zpVector4f origin = cursor;
    zpVector4f newLine = zpMath::Vector4( 0, static_cast<zp_float>( lineHeight + lineSpacing ), 0, 0 );
    zp_uint finalLineHeight = lineHeight;

    ZP_ALIGN16 zpVector4fData p0, p1, p2, p3;

    zp_ushort baseIndexOffset = static_cast<zp_ushort>( cmd->vertexCount );

    for( ; ( curr = *text ); ++text )
    {
        switch( curr )
        {
            case '\n':
                finalLineHeight += lineHeight + lineSpacing;
                cursor = zpMath::Vector4Add( cursor, newLine );
                cursor = zpMath::Vector4SetX( cursor, zpMath::Vector4GetX( origin ) );
                continue;

            case '\r':
                continue;
        }

        zpGlyph* g = font->fontGlyphs + curr;

        zpVector4f tl, tr, br, bl;

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

        zpMath::Vector4Store4( bl, p0.m );
        zpMath::Vector4Store4( tl, p1.m );
        zpMath::Vector4Store4( tr, p2.m );
        zpMath::Vector4Store4( br, p3.m );

        zpVector2f u0 = { g->uvRect.x,                   g->uvRect.y + g->uvRect.height };
        zpVector2f u1 = { g->uvRect.x,                   g->uvRect.y };
        zpVector2f u2 = { g->uvRect.x + g->uvRect.width, g->uvRect.y };
        zpVector2f u3 = { g->uvRect.x + g->uvRect.width, g->uvRect.y + g->uvRect.height };

        // bl -> tl -> tr -> br
        addVertexData( p0, bottomColor, u0 );
        addVertexData( p1, topColor, u1 );
        addVertexData( p2, topColor, u2 );
        addVertexData( p3, bottomColor, u3 );

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

zp_uint zpRenderingContext::addTextShadow( const zpVector4fData& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor, const zpVector4fData& shadowOffset, const zpColor32i& shadowColor )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    zp_char prev = '\0';
    zp_char curr = '\0';

    zpFont* font = cmd->font.get();

    const zp_float scale = static_cast<zp_float>( size ) / static_cast<zp_float>( font->baseFontSize );
    const zp_uint lineHeight = static_cast<zp_uint>( ( static_cast<zp_float>( size ) * scale ) + 1.f ) + lineSpacing;

    zpScalar sScale = zpMath::Scalar( scale );

    ZP_ALIGN16 zpVector4fData p = pos;
    zpVector4f cursor = zpMath::Vector4Load4( p.m );
    zpVector4f origin = cursor;
    zpVector4f newLine = zpMath::Vector4( 0, static_cast<zp_float>( lineHeight ), 0, 0 );
    zp_uint finalLineHeight = lineHeight;

    p = shadowOffset;
    zpVector4f shadow = zpMath::Vector4Load4( p.m );

    ZP_ALIGN16 zpVector4fData p0, p1, p2, p3;

    zp_ushort baseIndexOffset = static_cast<zp_ushort>( cmd->vertexCount );

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

        zpGlyph* g = font->fontGlyphs + curr;

        zpVector4f tl, tr, br, bl;
        zpVector4f stl, str, sbr, sbl;

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
        addVertexData( p0, shadowColor, u0 );
        addVertexData( p1, shadowColor, u1 );
        addVertexData( p2, shadowColor, u2 );
        addVertexData( p3, shadowColor, u3 );

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
        addVertexData( p0, bottomColor, u0 );
        addVertexData( p1, topColor, u1 );
        addVertexData( p2, topColor, u2 );
        addVertexData( p3, bottomColor, u3 );

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

void zpRenderingContext::endDraw()
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->vertexOffset = m_immediateVertexSize;
    cmd->indexOffset = m_immediateIndexSize;

    m_immediateVertexSize = m_scratchVertexBuffer.getPosition();
    m_immediateIndexSize = m_scratchIndexBuffer.getPosition();

    m_currentCommnad = npos;
}

void zpRenderingContext::drawMesh( zp_byte layer, const zpMatrix4fData& transformData, const zpMeshHandle& mesh, const zpMaterialHandle& material )
{
    ZP_ASSERT( m_currentCommnad == npos, "" );

    const zpMesh* m = mesh.get();
    for( zp_size_t i = 0, imax = m->numMeshParts; i < imax; ++i )
    {
        const zpMeshPart* p = m->parts + i;

        zpRenderingCommand& cmd = m_commands.pushBackEmpty();
        cmd.type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
        cmd.sortKey.key = 0;
        cmd.sortKey.layer = layer;
        cmd.topology = ZP_TOPOLOGY_TRIANGLE_LIST;
        cmd.vertexFormat = m->vertexFormat;
        cmd.vertexOffset = p->vertexOffset;
        cmd.vertexCount = p->vertexCount;
        cmd.indexOffset = p->indexOffset;
        cmd.indexCount = p->indexCount;
        cmd.vertexBuffer = m->vertexData;
        cmd.indexBuffer = m->indexData;
        cmd.material = material;
        cmd.font = zpFontHandle();
        cmd.mesh = mesh;
        cmd.transform = transformData;
    }
}

void zpRenderingContext::setTransform( zpMatrix4fParamF transform )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( transform, transformData.m );
    cmd->transform = transformData;
}

void zpRenderingContext::setTransform( const zpMatrix4fData& transform )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->transform = transform;
}

void zpRenderingContext::setTransform( zp_size_t cmdOffset, zpMatrix4fParamF transform )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    ZP_ASSERT( ( m_currentCommnad + cmdOffset ) < m_commands.size(), "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad + cmdOffset;

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( transform, transformData.m );
    cmd->transform = transformData;
}

void zpRenderingContext::setTransform( zp_size_t cmdOffset, const zpMatrix4fData& transform )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    ZP_ASSERT( ( m_currentCommnad + cmdOffset ) < m_commands.size(), "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad + cmdOffset;

    cmd->transform = transform;
}

void zpRenderingContext::setMaterial( const zpMaterialHandle& material )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->material = material;
}

void zpRenderingContext::setMaterial( zp_size_t cmdOffset, const zpMaterialHandle& material )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    ZP_ASSERT( ( m_currentCommnad + cmdOffset ) < m_commands.size(), "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad + cmdOffset;

    cmd->material = material;
}

zp_size_t zpRenderingContext::getVertexCount() const
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    const zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    return cmd->vertexCount;
}

zp_size_t zpRenderingContext::getVertexCount( zp_size_t cmdOffset ) const
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    ZP_ASSERT( ( m_currentCommnad + cmdOffset ) < m_commands.size(), "" );
    const zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad + cmdOffset;

    return cmd->vertexCount;
}

void zpRenderingContext::addVertex( zpVector4fParamF pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    ZP_ALIGN16 zpVector4fData v;
    zpMath::Vector4Store4( pos, v.m );

    struct
    {
        zpVector4fData p;
        zpColor32i c;
    } buff = {
        v,
        color,
    };
    
    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addVertexData( const zpVector4fData& pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    struct
    {
        zpVector4fData p;
        zpColor32i c;
    } buff = {
        pos,
        color,
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addVertexData( const zpVector4fData& pos, const zpColor32i& color, const zpVector2f& uv )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

    struct
    {
        zpVector4fData p;
        zpColor32i c;
        zpVector2f u;
    } buff = {
        pos,
        color,
        uv,
    };

    m_scratchVertexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    const zp_ushort buff[] = {
        index0,
        index1,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->indexCount += 2;
}

void zpRenderingContext::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    const zp_ushort buff[] = {
        index0,
        index1,
        index2,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->indexCount += 3;
}

void zpRenderingContext::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    const zp_ushort buff[] = {
        index0,
        index1,
        index2,

        index2,
        index3,
        index0,
    };

    m_scratchIndexBuffer.write( &buff, 0, sizeof( buff ) );
    cmd->indexCount += 6;
}

void zpRenderingContext::fillBuffers()
{
    ZP_PROFILER_BLOCK();

    if( m_scratchVertexBuffer.getPosition() )
    {
        SetRenderBufferDataOpenGL( m_immidateVertexBuffers[ m_currentBufferIndex ], m_scratchVertexBuffer.getBuffer(), 0, m_scratchVertexBuffer.getPosition() );
    }

    if( m_scratchIndexBuffer.getPosition() )
    {
        SetRenderBufferDataOpenGL( m_immidateIndexBuffers[ m_currentBufferIndex ], m_scratchIndexBuffer.getBuffer(), 0, m_scratchIndexBuffer.getPosition() );
    }

    m_scratchVertexBuffer.clear();
    m_scratchIndexBuffer.clear();
}

void zpRenderingContext::processCommands( zpRenderingCommandProcessFunc func )
{
    ZP_PROFILER_BLOCK();
    
    zpRenderingCommand* cmd = m_commands.begin();
    zpRenderingCommand* end = m_commands.end();
    for( ; cmd != end; ++cmd )
    {
        func( cmd );
    }
}

void zpRenderingContext::flipBuffers()
{
    ZP_PROFILER_BLOCK();

    m_commands.clear();

    m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % 2;

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;
}

const zpVector< zpRenderingCommand >& zpRenderingContext::getCommands() const
{
    return m_commands;
}