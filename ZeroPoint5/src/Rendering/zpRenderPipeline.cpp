#include "Rendering\zpRendering.h"

zpRenderPipelinePass::zpRenderPipelinePass()
{

}

zpRenderPipelinePass::~zpRenderPipelinePass()
{

}

void zpRenderPipelinePass::update()
{
    onUpdate();
}

void zpRenderPipelinePass::executePass( const zpCamera* camera, zpRenderCommandBuffer* cmdBuffer )
{
    onRender( camera, cmdBuffer );
}

//
//
//

zpRenderPipeline::zpRenderPipeline()
{

}

zpRenderPipeline::~zpRenderPipeline()
{

}

void zpRenderPipeline::addPass( zpRenderPipelinePass* pass )
{

}

void zpRenderPipeline::executePipeline( const zpCamera* camera, zpRenderCommandBuffer* cmdBuffer )
{
    zpVector< zpRenderPipelinePass* >::iterator b = m_pipeline.begin();
    zpVector< zpRenderPipelinePass* >::iterator e = m_pipeline.end();

    for( ; b != e; ++b )
    {
        zpRenderPipelinePass* pass = *b;
        pass->executePass( camera, cmdBuffer );
    }
}

void zpRenderPipeline::clear()
{
    m_pipeline.clear();
}
