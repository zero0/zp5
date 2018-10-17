#include "Rendering\zpRendering.h"

zpRenderPipelinePass::zpRenderPipelinePass()
    : m_flags( zp_flag32_mark( 0, ZP_RENDER_PIPELINE_PASS_FLAG_IS_ENABLED ) )
{
}

zpRenderPipelinePass::~zpRenderPipelinePass()
{
}

zp_bool zpRenderPipelinePass::isEnabled() const
{
    return zp_flag32_is_set( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_ENABLED );
}

void zpRenderPipelinePass::setEnabled( zp_bool enabled )
{
    const zp_bool wasEnabled = zp_flag32_is_set( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_ENABLED );
    m_flags = zp_flag32_set( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_ENABLED, enabled );
    if( wasEnabled != enabled )
    {
        markAsDirty();
    }
}

zp_bool zpRenderPipelinePass::isDirty() const
{
    return zp_flag32_is_set( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_DIRTY );
}

void zpRenderPipelinePass::markAsDirty()
{
    m_flags = zp_flag32_mark( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_DIRTY );
}

void zpRenderPipelinePass::markAsClean()
{
    m_flags = zp_flag32_clear( m_flags, ZP_RENDER_PIPELINE_PASS_FLAG_IS_DIRTY );
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

