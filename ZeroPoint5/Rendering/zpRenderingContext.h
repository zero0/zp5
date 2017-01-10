#pragma once
#ifndef ZP_RENDERING_CONTEXT_H
#define ZP_RENDERING_CONTEXT_H

class zpRenderingContext
{
public:
    zpRenderingContext();
    ~zpRenderingContext();

    void clear( const zpColor& clearColor, zp_float clearDepth, zp_uint clearStencil );
    void flush();

    void setViewport( const zpViewport& viewport );
    void setScissorRect( const zpRecti& scissorRect );
    void resetScissorRect();

    void beginDrawImmediate( zp_byte layer, zpTopology topology, zpVertexFormat vertexFormat );
    void endDrawImmediate();

    void fillBuffers();
    void flipBuffers();

private:
    zp_size_t m_currentCommnad;

    zpVector< zpRenderingCommand > m_commands;

    zpDataBuffer m_scratchVertexBuffer;
    zpDataBuffer m_scratchIndexBuffer;

    zp_size_t m_currentBufferIndex;

    zp_size_t m_immediateVertexSize;
    zp_size_t m_immediateIndexSize;

};

#endif // !ZP_RENDERING_CONTEXT_H
