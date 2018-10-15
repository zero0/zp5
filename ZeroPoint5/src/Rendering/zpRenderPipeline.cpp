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

void zpRenderPipelinePass::executePass( const zpCamera* camera, zpRenderContext* renderContext )
{
    onExecutePipeline( camera, renderContext );
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

void zpRenderPipeline::executePipeline( const zpCamera* camera, zpRenderContext* renderContext )
{
    zpVector< zpRenderPipelinePass* >::iterator b = m_pipeline.begin();
    zpVector< zpRenderPipelinePass* >::iterator e = m_pipeline.end();

    for( ; b != e; ++b )
    {
        zpRenderPipelinePass* pass = *b;
        pass->executePass( camera, renderContext );
    }
}

