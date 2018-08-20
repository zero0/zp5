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

void zpRenderCommandBuffer::clearColorDepthStencil( const zpColorf& clearColor, zp_float clearDepth, zp_uint clearStencil )
{
    zpRenderCommandClear cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_COLOR_DEPTH_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;
    cmd.depth = clearDepth;
    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}
void zpRenderCommandBuffer::clearColorDepth( const zpColorf& clearColor, zp_float clearDepth )
{
    zpRenderCommandClearColorDepth cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_COLOR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;
    cmd.depth = clearDepth;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearDepthStencil( zp_float clearDepth, zp_uint clearStencil )
{
    zpRenderCommandClearDepthStencil cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_DEPTH_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.depth = clearDepth;
    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearColor( const zpColorf& clearColor )
{
    zpRenderCommandClearColor cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_COLOR;
    cmd.header.id = ++m_uniqueID;

    cmd.color = clearColor;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearDepth( zp_float clearDepth )
{
    zpRenderCommandClearDepth cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.depth = clearDepth;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::clearStencil( zp_uint clearStencil )
{
    zpRenderCommandClearStencil cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_CLEAR_STENCIL;
    cmd.header.id = ++m_uniqueID;

    cmd.stencil = clearStencil;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTarget( const zpRenderTargetIdentifier& colorTarget )
{
    zpRenderCommandSetRenderTargetColor cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_RT_COLOR;
    cmd.header.id = ++m_uniqueID;

    cmd.rtColor = colorTarget;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTarget( const zpRenderTargetIdentifier& colorTarget, const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetColorDepth cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_RT_COLOR_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtColor = colorTarget;
    cmd.rtDepth = depthTarget;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount )
{
    zpRenderCommandSetRenderTargetColors cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_RT_COLORS;
    cmd.header.id = ++m_uniqueID;

    cmd.rtCount = colorCount;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setRenderTargets( const zpRenderTargetIdentifier* colorTargets, zp_size_t colorCount, const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetColorsDepth cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_RT_COLORS_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtCount = colorCount;
    cmd.rtDepth = depthTarget;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setDepthRenderTarget( const zpRenderTargetIdentifier& depthTarget )
{
    zpRenderCommandSetRenderTargetDepth cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_RT_DEPTH;
    cmd.header.id = ++m_uniqueID;

    cmd.rtDepth = depthTarget;
    
    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setViewport( const zpViewport& viewport )
{
    zpRenderCommandSetViewport cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_VIEWPORT;
    cmd.header.id = ++m_uniqueID;

    cmd.viewport = viewport;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::setScissorRect( const zpRecti& scissorRect )
{
    zpRenderCommandSetScissorRect cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_SET_SCISSOR_RECT;
    cmd.header.id = ++m_uniqueID;

    cmd.scissorRect = scissorRect;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::drawRenderers( const zpDrawRenderersDesc& desc )
{
    zpRenderCommandDrawRenderers cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_DRAW_RENDERERS;
    cmd.header.id = ++m_uniqueID;

    cmd.desc = desc;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::blit( const zpRenderTargetIdentifier& src, const zpRenderTargetIdentifier& dest )
{
    zpRenderCommandBlitRenderTexture cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_BLIT_RT;
    cmd.header.id = ++m_uniqueID;

    m_buffer.write( cmd );
}

void zpRenderCommandBuffer::blit( const zpTexture& src, const zpRenderTargetIdentifier& dest )
{
    zpRenderCommandBlitTexture cmd;
    cmd.header.type = ZP_RENDER_COMMNAD_BLIT_TEXTURE;
    cmd.header.id = ++m_uniqueID;

    m_buffer.write( cmd );
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
    m_buffer.reset();
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
