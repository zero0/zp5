#include "zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

zpRenderingContext::zpRenderingContext()
    : m_currentCommnad( static_cast< zp_size_t >( -1 ) )
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

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( zpMath::MatrixIdentity(), transformData.m );
    cmd.transform = transformData;
}

void zpRenderingContext::endDrawImmediate()
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->vertexOffset = m_immediateVertexSize;
    cmd->indexOffset = m_immediateIndexSize;
    
    m_immediateVertexSize = m_scratchVertexBuffer.getPosition();
    m_immediateIndexSize = m_scratchIndexBuffer.getPosition();

    m_currentCommnad = npos;
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

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( zpMath::MatrixIdentity(), transformData.m );
    cmd.transform = transformData;
}

void zpRenderingContext::addText( const zpVector4fData& pos, const zp_char* text, zp_uint size, const zpColor32i& topColor, const zpColor32i& bottomColor )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    zp_char prev = '\0';
    zp_char curr = '\0';

    zpFont* font = cmd->font.operator->();

    zpScalar scale = zpMath::Scalar( static_cast<zp_float>( size ) / static_cast<zp_float>( 12 ) );

    ZP_ALIGN16 zpVector4fData p = pos;
    zpVector4f cursor = zpMath::Vector4Load4( p.m );
    zpVector4f origin = cursor;
    zpVector4f newLine = zpMath::Vector4Scale( zpMath::Vector4( 0, 12, 0, 0 ), scale );

    ZP_ALIGN16 zpVector4fData p0, p1, p2, p3;
    zpVector2f u0, u1, u2, u3;

    zp_ushort baseIndexOffset = static_cast<zp_ushort>( cmd->vertexCount );

    for( ; *text ; ++text )
    {
        curr = *text;

        switch( curr )
        {
            case '\n':
                cursor = zpMath::Vector4Add( cursor, newLine );
                cursor = zpMath::Vector4SetX( cursor, zpMath::Vector4GetX( origin ) );
                continue;

            case '\r':
                continue;
        }

        zpGlyph* g = font->fontGlyphs + curr;

        zpVector4f tl, tr, br, bl;

        zp_int minX = g->bearing.x;
        zp_int maxX = g->bearing.x + g->size.x;
        zp_int minY = -( g->size.y - g->bearing.y );
        zp_int maxY = g->bearing.y;
        
        bl = zpMath::Vector4Scale( zpMath::Vector4( minX, maxY, 0, 0 ), scale );
        bl = zpMath::Vector4Add( cursor, bl );
        zpMath::Vector4Store4( bl, p0.m );

        tl = zpMath::Vector4Scale( zpMath::Vector4( minX, minY, 0, 0 ), scale );
        tl = zpMath::Vector4Add( cursor, tl );
        zpMath::Vector4Store4( tl, p1.m );
        
        tr = zpMath::Vector4Scale( zpMath::Vector4( maxX, minY, 0, 0 ), scale );
        tr = zpMath::Vector4Add( cursor, tr );
        zpMath::Vector4Store4( tr, p2.m );

        br = zpMath::Vector4Scale( zpMath::Vector4( maxX, maxY, 0, 0 ), scale );
        br = zpMath::Vector4Add( cursor, br );
        zpMath::Vector4Store4( br, p3.m );

        u0 = { g->uvRect.x,                   g->uvRect.y };
        u1 = { g->uvRect.x,                   g->uvRect.y + g->uvRect.height };
        u2 = { g->uvRect.x + g->uvRect.width, g->uvRect.y + g->uvRect.height };
        u3 = { g->uvRect.x + g->uvRect.width, g->uvRect.y };

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

        cursor = zpMath::Vector4Add( cursor, zpMath::Vector4Scale( zpMath::Vector4( g->advance, 0, 0, 0 ), scale ) );

        prev = curr;
    }
}

void zpRenderingContext::endDrawText()
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->vertexOffset = m_immediateVertexSize;
    cmd->indexOffset = m_immediateIndexSize;

    m_immediateVertexSize = m_scratchVertexBuffer.getPosition();
    m_immediateIndexSize = m_scratchIndexBuffer.getPosition();

    m_currentCommnad = npos;
}

void zpRenderingContext::setTransform( zpMatrix4fParamF transform )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    ZP_ALIGN16 zpMatrix4fData transformData;
    zpMath::MatrixStore4( transform, transformData.m );
    cmd->transform = transformData;
}

void zpRenderingContext::setMaterial( const zpMaterialHandle& material )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad; sizeof( zpRenderingCommand );
    cmd->material = material;
}

void zpRenderingContext::addVertex( zpVector4fParamF pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    ZP_ALIGN16 zp_float v[ 4 ];
    zpMath::Vector4Store4( pos, v );

    m_scratchVertexBuffer.write( v, 0, sizeof( v ) );
    m_scratchVertexBuffer.write( &color, 0, sizeof( zpColor32i ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addVertexData( const zpVector4fData& pos, const zpColor32i& color )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    m_scratchVertexBuffer.write( &pos, 0, sizeof( zpVector4fData ) );
    m_scratchVertexBuffer.write( &color, 0, sizeof( zpColor32i ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addVertexData( const zpVector4fData& pos, const zpColor32i& color, const zpVector2f& uv )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR_UV, "" );

    m_scratchVertexBuffer.write( &pos, 0, sizeof( zpVector4fData ) );
    m_scratchVertexBuffer.write( &color, 0, sizeof( zpColor32i ) );
    m_scratchVertexBuffer.write( &uv, 0, sizeof( zpVector2f ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 2;
}

void zpRenderingContext::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index2, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 3;
}

void zpRenderingContext::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
{
    ZP_ASSERT( m_currentCommnad != npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index2, 0, sizeof( zp_ushort ) );

    m_scratchIndexBuffer.write( &index2, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index3, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
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
    m_commands.clear();

    m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % 2;

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;
}
