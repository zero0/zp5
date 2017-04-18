#pragma once
#ifndef ZP_RENDERING_CONTEXT_H
#define ZP_RENDERING_CONTEXT_H

typedef void(zpRenderingCommandProcessFunc)( zpRenderingCommand* );

class zpRenderingContext
{
public:

    zpRenderingContext();
    ~zpRenderingContext();

    void setup();
    void teardown();

    void clear( const zpColorf& clearColor, zp_float clearDepth, zp_uint clearStencil );
    void flush();

    void setViewport( const zpViewport& viewport );
    void setScissorRect( const zpRecti& scissorRect );
    void resetScissorRect();

    void beginDrawImmediate( zp_byte layer, zpTopology topology, zpVertexFormat vertexFormat );
    void beginDrawText( zp_byte layer, const zpFontHandle& font );
    void beginDrawMesh( zp_byte layer, const zpMatrix4fData& transformData, const zpMeshHandle& mesh, const zpMaterialHandle& material );
    void endDraw();

    void setTransform( zpMatrix4fParamF transform );
    void setTransform( const zpMatrix4fData& transform );
    void setTransform( zp_size_t cmdOffset, zpMatrix4fParamF transform );
    void setTransform( zp_size_t cmdOffset, const zpMatrix4fData& transform );

    void setMaterial( const zpMaterialHandle& material );
    void setMaterial( zp_size_t cmdOffset, const zpMaterialHandle& material );

    void addText( const zpVector4fData& pos, const zp_char* text, zp_uint size, const zpColor32i& topColor, const zpColor32i& bottomColor );
    
    void addVertex( zpVector4fParamF pos, const zpColor32i& color );

    void addVertexData( const zpVector4fData& pos, const zpColor32i& color );
    void addVertexData( const zpVector4fData& pos, const zpColor32i& color, const zpVector2f& uv );

    void addLineIndex( zp_ushort index0, zp_ushort index1 );
    void addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 );
    void addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 );

    void fillBuffers();
    void processCommands( zpRenderingCommandProcessFunc func );
    void flipBuffers();

    const zpVector< zpRenderingCommand >& getCommands() const;

private:
    static const zp_size_t npos = static_cast<zp_size_t>( -1 );
    zp_size_t m_currentCommnad;

    zpVector< zpRenderingCommand > m_commands;

    zpDataBuffer m_scratchVertexBuffer;
    zpDataBuffer m_scratchIndexBuffer;

    zpRenderBuffer m_immidateVertexBuffers[ 2 ];
    zpRenderBuffer m_immidateIndexBuffers[ 2 ];

    zp_size_t m_currentBufferIndex;

    zp_size_t m_immediateVertexSize;
    zp_size_t m_immediateIndexSize;

};

#endif // !ZP_RENDERING_CONTEXT_H
