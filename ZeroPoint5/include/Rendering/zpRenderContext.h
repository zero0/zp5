#pragma once
#ifndef ZP_RENDER_CONTEXT_H
#define ZP_RENDER_CONTEXT_H

//
//
//

struct zpDrawRenderable
{
    zp_flag32 renderLayers;
    zp_int passIndex;
    zp_int subMeshIndex;

    zpMeshHandle mesh;
    zpMaterialHandle material;

    zpRenderBuffer vertexBuffer;
    zpRenderBuffer indexBuffer;
    zp_size_t vertexOffset;
    zp_size_t vertexCount;
    zp_size_t indexOffset;
    zp_size_t indexCount;
    zpIndexStride indexStride;
    zpVertexFormat vertexFormat;
    zpTopology topology;

    zpMatrix4f localToWorld;
    zpBoundingAABB bounds;
};

struct zpDrawRenderableSort
{
    zpRenderSortKey sortKey;
    const zpDrawRenderable* drawRenderable;
};

class zpRenderContext
{
public:
    zpRenderContext();
    ~zpRenderContext();

    void addDrawRenderable( const zpDrawRenderable& renderable );
    void clearDrawRenderables();

    void drawRenderers( const zpDrawRenderersDesc& desc );

    void executeCommandBuffer( const zpRenderCommandBuffer& buffer );

    void setActiveCamera( const zpCamera* camera );
    const zpCamera* getActiveCamera() const;

    zpRenderCommandBuffer* getCommandBuffer();

    void submit();

private:
    const zpCamera* m_currentCamera;

    zpRenderCommandBuffer m_commandBuffer;
    zpVector< zpDrawRenderable > m_allRenderables;
    zpVector< zpDrawRenderableSort > m_renderableSort;
};

#endif // !ZP_RENDER_CONTEXT_H
