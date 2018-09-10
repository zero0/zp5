#pragma once
#ifndef ZP_RENDER_CONTEXT_H
#define ZP_RENDER_CONTEXT_H

#include "Rendering\zpRendering.h"

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
