#pragma once
#ifndef ZP_RENDER_PIPELINE_H
#define ZP_RENDER_PIPELINE_H

enum zpRenderPipelinePassFlags : zp_flag32
{
    ZP_RENDER_PIPELINE_PASS_FLAG_IS_ENABLED,
    ZP_RENDER_PIPELINE_PASS_FLAG_IS_DIRTY,

    zpRenderPipelinePass_Count,
    zpRenderPipelinePass_UserStart,

    zpRenderPipelinePass_Force32 = ZP_FORCE_32BIT
};

enum zpRenderPipelinePassResolveResult : zp_uint
{
    ZP_RENDER_PIPELINE_PASS_RESOLVE_FAILED,
    ZP_RENDER_PIPELINE_PASS_RESOLVE_SUCCESS,

};

ZP_ABSTRACT_CLASS zpRenderPipelinePass
{
public:
    zpRenderPipelinePass();
    virtual ~zpRenderPipelinePass();

    zp_bool isEnabled() const;
    void setEnabled( zp_bool enabled );

    zp_bool isDirty() const;
    void markAsDirty();

public:
    virtual zpRenderPipelinePassResolveResult resolve() = 0;

    virtual void update( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

    virtual void executePass( const zpCamera* camera, zpRenderContext* renderContext )
    {
    }

protected:
    void markAsClean();

private:
    zp_flag32 m_flags;
};

//
//
//

class zpRenderPipeline
{
public:
    zpRenderPipeline();
    ~zpRenderPipeline();

    void update( const zpCamera* camera, zpRenderContext* renderContext );
    void executePipeline( const zpCamera* camera, zpRenderContext* renderContext );

    void rebuild();

    template<typename T, typename Allocator = zpDefaultGlobalAllocator>
    T* addPass()
    {
        Allocator alloc;
        void* v = alloc.allocate( sizeof( T ) );
        T* pass = new ( v ) T();
        zpRenderPipelinePass* p = static_cast<zpRenderPipelinePass*>( pass );
        m_allPasses.pushBack( p );
        return pass;
    }

    template<typename Allocator = zpDefaultGlobalAllocator>
    void clearAllPasses()
    {
        m_pipeline.clear();

        Allocator alloc;
        zpVector< zpRenderPipelinePass* >::iterator b = m_allPasses.begin();
        zpVector< zpRenderPipelinePass* >::iterator e = m_allPasses.end();

        for( ; b != e; ++b )
        {
            zpRenderPipelinePass* pass = *b;
            alloc.free( pass );
        }

        m_allPasses.clear();
    }
protected:
    void buildPipeline();

private:
    zpVector< zpRenderPipelinePass* > m_pipeline;
    zpVector< zpRenderPipelinePass* > m_allPasses;
};

#endif // !ZP_RENDER_PIPELINE_H
