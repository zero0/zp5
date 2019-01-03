#pragma once
#ifndef ZP_RENDER_IMMEDIATE_H
#define ZP_RENDER_IMMEDIATE_H

struct zpDrawImmediateDesc
{

};

struct zpDrawTextDesc
{
    const zp_char* text;
    zp_size_t length;
    
    zp_uint size;
    zp_uint lineSpacing;
    
    zpVector4f position;
    zpColorf topColor;
};

class zpRenderImmediate
{
public:
    zpRenderImmediate();
    ~zpRenderImmediate();

    void setup( zpRenderingEngine* engine, zpRenderContext* ctx );
    void teardown();

    void beginDrawImmediate( zpTopology topology, zpVertexFormat vertexFormat );
    
    void beginDrawImmediate( zpMatrix4fParamF transform, zpTopology topology, zpVertexFormat vertexFormat );
    void beginDrawImmediate( const zpMatrix4f& transform, zpTopology topology, zpVertexFormat vertexFormat );
    
    void beginDrawImmediate( zpMatrix4fParamF transform, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialHandle& material );
    void beginDrawImmediate( const zpMatrix4f& transform, zpTopology topology, zpVertexFormat vertexFormat, const zpMaterialHandle& material );

    void addVertex( zpVector4fParamF pos, const zpColor32i& color );
    void addVertex( zpVector4fParamF pos, const zpColor32i& color, const zpVector2f& uv );

    void addVertex( const zpVector4f& pos, const zpColor32i& color );
    void addVertex( const zpVector4f& pos, const zpColor32i& color, const zpVector2f& uv );

    void addLineIndex( zp_ushort index0, zp_ushort index1 );
    void addTriangleIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2 );
    void addQuadIndex( zp_ushort index0, zp_ushort index1, zp_ushort index2, zp_ushort index3 );

    void setMaterial( const zpMaterialHandle& material );
    void setTransform( zpMatrix4fParamF transform );
    void setTransform( const zpMatrix4f& transform );

    void endDrawImmediate();

    void beginDrawText( const zpFontHandle& font );

    zp_uint addText( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor );
    zp_uint addTextShadow( const zpVector4f& pos, const zp_char* text, zp_uint size, zp_uint lineSpacing, const zpColor32i& topColor, const zpColor32i& bottomColor, const zpVector4f& shadowOffset, const zpColor32i& shadowColor );

    void endDrawText();

    zp_size_t getVertexCount() const;
    zp_size_t getIndexCount() const;

    void processCommands( zpRenderCommandBuffer* buffer );
    void flipBuffers();

private:
    zpFontHandle m_currentFont;
    zpMaterialHandle m_currentMaterial;

    zpRenderingEngine* m_engine;
    zpRenderContext* m_ctx;

    zpRenderBuffer m_immidateVertexBuffers[ 2 ];
    zpRenderBuffer m_immidateIndexBuffers[ 2 ];

    zpMatrix4f m_currentTransform;
    zpTopology m_currentTopology;
    zpVertexFormat m_currentVertexFormat;
    zp_size_t m_currentVertexCount;
    zp_size_t m_currentIndexCount;

    zpDataBuffer m_scratchVertexBuffer;
    zpDataBuffer m_scratchIndexBuffer;

    zp_size_t m_currentBufferIndex;

    zp_size_t m_immediateVertexSize;
    zp_size_t m_immediateIndexSize;
};

#endif // !ZP_RENDER_IMMEDIATE_H
