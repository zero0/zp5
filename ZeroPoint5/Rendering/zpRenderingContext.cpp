#include "zpRendering.h"

zpRenderingContext::zpRenderingContext()
{
}

zpRenderingContext::~zpRenderingContext()
{
}

void zpRenderingContext::clear( const zpColor& clearColor, zp_float clearDepth, zp_uint clearStencil )
{
    ZP_ASSERT( m_currentCommnad == -1, "" );

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
    ZP_ASSERT( m_currentCommnad == -1, "" );

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_SET_VIEWPORT;
    cmd.sortKey.key = 0;
    cmd.sortKey.anchor = true;
    cmd.viewport = viewport;
}

void zpRenderingContext::setScissorRect( const zpRecti& scissorRect )
{
    ZP_ASSERT( m_currentCommnad == -1, "" );

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
    ZP_ASSERT( m_currentCommnad == -1, "" );
    m_currentCommnad = m_commands.size();

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_DRAW_IMMEDIATE;
    cmd.sortKey.key = 0;
    cmd.sortKey.layer = layer;
    cmd.topology = topology;
    cmd.vertexOffset = 0;
    cmd.vertexCount = 0;
    cmd.indexOffset = 0;
    cmd.indexCount = 0;
    cmd.transform = zpMath::MatrixIdentity();
}

void zpRenderingContext::endDrawImmediate()
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->vertexOffset = m_immediateVertexSize;
    cmd->indexOffset = m_immediateIndexSize;
    
    m_immediateVertexSize = m_scratchVertexBuffer.getPosition();
    m_immediateIndexSize = m_scratchIndexBuffer.getPosition();

    m_currentCommnad = -1;
}

void zpRenderingContext::setTransform( zpMatrix4fParamF transform )
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    cmd->transform = transform;
}

void zpRenderingContext::addVertex( zpVector4fParamF pos, const zpColor& color )
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    ZP_ALIGN16 zp_float v[ 4 ];
    zpMath::Vector4Store4( pos, v );

    m_scratchVertexBuffer.write( v, 0, sizeof( v ) );
    m_scratchVertexBuffer.write( &color, 0, sizeof( zpColor ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 2;
}

void zpRenderingContext::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index2, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 3;
}

void zpRenderingContext::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
{
    ZP_ASSERT( m_currentCommnad != -1, "" );
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
    m_scratchVertexBuffer.reset();
    m_scratchIndexBuffer.reset();
}

void zpRenderingContext::processCommands( zpRenderingCommandProcessFunc func )
{
    zpRenderingCommand* cmd = m_commands.begin();
    zpRenderingCommand* end = m_commands.end();
    for( ; cmd != end; ++cmd )
    {
        func( cmd );
    }
}

void zpRenderingContext::flipBuffers()
{
    m_commands.reset();

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;
}
