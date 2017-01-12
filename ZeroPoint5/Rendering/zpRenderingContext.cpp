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
    CreateRenderBufferOpenGL( ZP_NULL, ZP_MEMORY_MB( 1 ), ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_DYNAMIC, m_immidateVertexBuffers[ 0 ] );
    CreateRenderBufferOpenGL( ZP_NULL, ZP_MEMORY_MB( 1 ), ZP_BUFFER_TYPE_VERTEX, ZP_BUFFER_BIND_DYNAMIC, m_immidateVertexBuffers[ 1 ] );

    CreateRenderBufferOpenGL( ZP_NULL, ZP_MEMORY_MB( 1 ), ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, m_immidateIndexBuffers[ 0 ] );
    CreateRenderBufferOpenGL( ZP_NULL, ZP_MEMORY_MB( 1 ), ZP_BUFFER_TYPE_INDEX, ZP_BUFFER_BIND_DYNAMIC, m_immidateIndexBuffers[ 1 ] );
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
    ZP_ASSERT( m_currentCommnad == m_commands.npos, "" );

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
    ZP_ASSERT( m_currentCommnad == m_commands.npos, "" );

    zpRenderingCommand& cmd = m_commands.pushBackEmpty();
    cmd.type = ZP_RENDERING_COMMNAD_SET_VIEWPORT;
    cmd.sortKey.key = 0;
    cmd.sortKey.anchor = true;
    cmd.viewport = viewport;
}

void zpRenderingContext::setScissorRect( const zpRecti& scissorRect )
{
    ZP_ASSERT( m_currentCommnad == m_commands.npos, "" );

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
    ZP_ASSERT( m_currentCommnad == m_commands.npos, "" );
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
    cmd.transform = zpMath::MatrixIdentity();
    cmd.vertexBuffer = m_immidateVertexBuffers[ m_currentBufferIndex ];
    cmd.indexBuffer = m_immidateIndexBuffers[ m_currentBufferIndex ];
}

void zpRenderingContext::endDrawImmediate()
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    cmd->vertexOffset = m_immediateVertexSize;
    cmd->indexOffset = m_immediateIndexSize;
    
    m_immediateVertexSize = m_scratchVertexBuffer.getPosition();
    m_immediateIndexSize = m_scratchIndexBuffer.getPosition();

    m_currentCommnad = -1;
}

void zpRenderingContext::setTransform( zpMatrix4fParamF transform )
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    cmd->transform = transform;
}

void zpRenderingContext::addVertex( zpVector4fParamF pos, const zpColorf& color )
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;
    ZP_ASSERT( cmd->vertexFormat == ZP_VERTEX_FORMAT_VERTEX_COLOR, "" );

    ZP_ALIGN16 zp_float v[ 4 ];
    zpMath::Vector4Store4( pos, v );

    m_scratchVertexBuffer.write( v, 0, sizeof( v ) );
    m_scratchVertexBuffer.write( &color, 0, sizeof( zpColorf ) );
    cmd->vertexCount += 1;
}

void zpRenderingContext::addLineIndex( zp_ushort index0, zp_ushort index1 )
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 2;
}

void zpRenderingContext::addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 )
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
    zpRenderingCommand* cmd = m_commands.begin() + m_currentCommnad;

    m_scratchIndexBuffer.write( &index0, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index1, 0, sizeof( zp_ushort ) );
    m_scratchIndexBuffer.write( &index2, 0, sizeof( zp_ushort ) );
    cmd->indexCount += 3;
}

void zpRenderingContext::addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 )
{
    ZP_ASSERT( m_currentCommnad != m_commands.npos, "" );
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

    m_currentBufferIndex = ( m_currentBufferIndex + 1 ) % 2;

    m_immediateIndexSize = 0;
    m_immediateVertexSize = 0;
}
