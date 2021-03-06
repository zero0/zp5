#include "Rendering/zpRendering.h"


zpRenderCommandBuffer::zpRenderCommandBuffer()
    : m_buffer( 1024 * 4 )
    , m_uniqueID( 0 )
{
}

zpRenderCommandBuffer::zpRenderCommandBuffer( zp_size_t capacity )
    : m_buffer( capacity )
    , m_uniqueID( 0 )
{
}

zpRenderCommandBuffer::~zpRenderCommandBuffer()
{
}

void zpRenderCommandBuffer::pushMarker( const zp_char* marker )
{
#if ZP_DEBUG
    zpRenderCommandPushMarker cmd;
    cmd.header.type = ZP_RENDER_COMMAND_PUSH_MARKER;
    cmd.header.id = ++m_uniqueID;

    const zp_size_t len = zp_strlen( marker );
    cmd.length = ZP_MIN( len + 1, ZP_MAX_MARKER_NAME_SIZE );

    zp_strcpy( cmd.marker, cmd.length, marker );

    m_buffer.write( cmd );
#endif
}

void zpRenderCommandBuffer::popMarker()
{
#if ZP_DEBUG
    zpRenderCommandPopMarker cmd;
    cmd.header.type = ZP_RENDER_COMMAND_POP_MARKER;
    cmd.header.id = ++m_uniqueID;

    m_buffer.write( cmd );
#endif
}

void zpRenderCommandBuffer::clearColorDepthStencil( const zpColorf& clearColor, zp_float clearDepth, zp_uint clearStencil )
{
    zpRenderCommandClear cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_COLOR_DEPTH_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;
    cmd.depth = clearDepth;
    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}
void zpRenderCommandBuffer::clearColorDepth( const zpColorf& clearColor, zp_float clearDepth )
{
    zpRenderCommandClearColorDepth cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_COLOR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;
    cmd.depth = clearDepth;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearDepthStencil( zp_float clearDepth, zp_uint clearStencil )
{
    zpRenderCommandClearDepthStencil cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_DEPTH_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.depth = clearDepth;
    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearColor( const zpColorf& clearColor )
{
    zpRenderCommandClearColor cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_COLOR;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearDepth( zp_float clearDepth )
{
    zpRenderCommandClearDepth cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.depth = clearDepth;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearStencil( zp_uint clearStencil )
{
    zpRenderCommandClearStencil cmd;
    cmd.header.type = ZP_RENDER_COMMAND_CLEAR_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTarget( const zpRenderTargetIdentifier& colorTarget )
{
    zpRenderCommandSetRenderTargetColor cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RT_COLOR;
    cmd.header.id = ++m_uniqueID;

    cmd.rtColor = colorTarget;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTarget( const zpRenderTargetIdentifier& colorTarget, const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetColorDepth cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RT_COLOR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtColor = colorTarget;
    cmd.rtDepth = depthTarget;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount )
{
    zpRenderCommandSetRenderTargetColors cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RT_COLORS;
    cmd.header.id = ++m_uniqueID;

    cmd.rtCount = colorCount;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount, const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetColorsDepth cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RT_COLORS_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtCount = colorCount;
    cmd.rtDepth = depthTarget;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setDepthRenderTarget( const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetDepth cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RT_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtDepth = depthTarget;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::resolveAntiAlias( const zpRenderTargetIdentifier& srcTarget, const zpRenderTargetIdentifier& destTarget )
{
    zpRenderCommandResolveAntiAliasedRenderTarget cmd;
    cmd.header.type = ZP_RENDER_COMMAND_RESOLVE_ANTI_ALIAS_RT;
    cmd.header.id = ++m_uniqueID;

    cmd.rtAntiAliasSource = srcTarget;
    cmd.rtTarget = destTarget;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setViewport( const zpViewport& viewport )
{
    zpRenderCommandSetViewport cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_VIEWPORT;
    cmd.header.id = ++m_uniqueID;

    cmd.viewport = viewport;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setScissorRect( const zpRecti& scissorRect )
{
    zpRenderCommandSetScissorRect cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_SCISSOR_RECT;
    cmd.header.id = ++m_uniqueID;

    cmd.scissorRect = scissorRect;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setBlendState( const zpBlendState& state )
{
    zpRenderCommandSetBlendState cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_BLEND_STATE;
    cmd.header.id = ++m_uniqueID;

    cmd.blendState = state;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setDepthState( const zpDepthState& state )
{
    zpRenderCommandSetDepthState cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_DEPTH_STATE;
    cmd.header.id = ++m_uniqueID;

    cmd.depthState = state;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setStencilState( const zpStencilState& state )
{
    zpRenderCommandSetStencilState cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_STENCIL_STATE;
    cmd.header.id = ++m_uniqueID;

    cmd.stencilState = state;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRasterState( const zpRasterState& state )
{
    zpRenderCommandSetRasterState cmd;
    cmd.header.type = ZP_RENDER_COMMAND_SET_RASTER_STATE;
    cmd.header.id = ++m_uniqueID;

    cmd.rasterState = state;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::drawMesh( const zpMatrix4f& transform, const zpDrawMeshDesc& desc, const zpMaterial* material, zp_int passIndex )
{
    zpRenderCommandDrawMesh cmd;
    cmd.header.type = ZP_RENDER_COMMAND_DRAW_MESH;
    cmd.header.id = ++m_uniqueID;

    cmd.vertexBuffer = desc.vertexBuffer;
    cmd.indexBuffer = desc.indexBuffer;
    cmd.vertexOffset = desc.vertexOffset;
    cmd.vertexCount = desc.vertexCount;
    cmd.indexOffset = desc.indexOffset;
    cmd.indexCount = desc.indexCount;
    cmd.baseVertexIndex = desc.baseVertexIndex;
    cmd.indexStride = desc.indexStride;
    cmd.vertexFormat = desc.vertexFormat;
    cmd.topology = desc.topology;

    cmd.localToWorld = transform;

    cmd.material = material;
    cmd.passIndex = passIndex;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::drawMesh( const zpMatrix4f& transform, const zpMesh* mesh, zp_size_t subMeshIndex, const zpMaterial* material, zp_int passIndex )
{
    zpRenderCommandDrawMesh cmd;
    cmd.header.type = ZP_RENDER_COMMAND_DRAW_MESH;
    cmd.header.id = ++m_uniqueID;

    cmd.vertexBuffer = mesh->vertexData;
    cmd.indexBuffer = mesh->indexData;
    cmd.vertexOffset = mesh->parts[ subMeshIndex ].vertexOffset;
    cmd.vertexCount = mesh->parts[ subMeshIndex ].vertexCount;
    cmd.indexOffset = mesh->parts[ subMeshIndex ].indexOffset;
    cmd.indexCount = mesh->parts[ subMeshIndex ].indexCount;
    cmd.baseVertexIndex = mesh->parts[ subMeshIndex ].baseVertexIndex;
    cmd.indexStride = mesh->indexStride;
    cmd.vertexFormat = mesh->vertexFormat;
    cmd.topology = ZP_TOPOLOGY_TRIANGLE_LIST;

    cmd.localToWorld = transform;

    cmd.material = material;
    cmd.passIndex = passIndex;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::blit( const zpRenderTargetIdentifier& src, const zpRenderTargetIdentifier& dest )
{
    zpRenderCommandBlitRenderTexture cmd;
    cmd.header.type = ZP_RENDER_COMMAND_BLIT_RT;
    cmd.header.id = ++m_uniqueID;

    cmd.rtSrc = src;
    cmd.rtDst = dest;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::blit( const zpTexture& src, const zpRenderTargetIdentifier& dest )
{
    ZP_INVALID_CODE_PATH();

    zpRenderCommandBlitTexture cmd;
    cmd.header.type = ZP_RENDER_COMMAND_BLIT_TEXTURE;
    cmd.header.id = ++m_uniqueID;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::executeCommandBuffer( const zpRenderCommandBuffer& buffer )
{
    m_buffer.write( buffer.m_buffer.getBuffer(), 0, buffer.m_buffer.getLength() );
}

const zp_byte* zpRenderCommandBuffer::getBuffer() const
{
    return m_buffer.getBuffer();
}

zp_size_t zpRenderCommandBuffer::getLength() const
{
    return m_buffer.getLength();
}

void zpRenderCommandBuffer::clear()
{
    m_buffer.clear();
    m_uniqueID = 0;
}

void zpRenderCommandBuffer::reset()
{
    m_buffer.clear();
    m_uniqueID = 0;
}

void zpRenderCommandBuffer::reserve( zp_size_t size )
{
    m_buffer.reserve( size );
}

void zpRenderCommandBuffer::destroy()
{
    m_buffer.clear();
    m_uniqueID = 0;
}
