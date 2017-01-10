#pragma once
#ifndef ZP_RENDERING_CONTEXT_H
#define ZP_RENDERING_CONTEXT_H

typedef void(zpRenderingCommandProcessFunc)( zpRenderingCommand* );

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

    void setTransform( zpMatrix4fParamF transform );

    void addVertex( zpVector4fParamF pos, const zpColor& color );

    void addLineIndex( zp_ushort index0, zp_ushort index1 );
    void addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 );
    void addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 );

    void fillBuffers();
    void processCommands( zpRenderingCommandProcessFunc func );
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
