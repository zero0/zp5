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
    cmd.clearDepth;
    cmd.clearStencil;
}

void zpRenderingContext::flush()
{

}

void zpRenderingContext::setViewport( const zpViewport& viewport )
{

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
    m_currentCommnad = -1;
}

void zpRenderingContext::fillBuffers()
{
    
}

void zpRenderingContext::flipBuffers()
{
    m_commands.reset();

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;
}
