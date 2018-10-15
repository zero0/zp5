#pragma once
#ifndef ZP_RENDER_PIPELINE_H
#define ZP_RENDER_PIPELINE_H

ZP_ABSTRACT_CLASS zpRenderPipelinePass
{
public:
    zpRenderPipelinePass();
    virtual ~zpRenderPipelinePass();

    void update();

    void executePass( const zpCamera* camera, zpRenderContext* renderContext );

protected:
    virtual void onUpdate()
    {
    }

    virtual void onExecutePipeline( const zpCamera* camera, zpRenderContext* renderContext ) = 0;

private:
};

class zpRenderPipeline
{
public:
    zpRenderPipeline();
    ~zpRenderPipeline();

    void executePipeline( const zpCamera* camera, zpRenderContext* renderContext );

    template<typename T, typename Allocator = zpDefaultGlobalAllocator>
    T* addPass()
    {
        Allocator alloc;
        void* v = alloc.allocate( sizeof( T ) );
        T* pass = new ( v ) T();
        zpRenderPipelinePass* p = static_cast<zpRenderPipelinePass*>( pass );
        m_pipeline.pushBack( p );
        return pass;
    }

    template<typename Allocator = zpDefaultGlobalAllocator>
    void clear()
    {
        Allocator alloc;
        zpVector< zpRenderPipelinePass* >::iterator b = m_pipeline.begin();
        zpVector< zpRenderPipelinePass* >::iterator e = m_pipeline.end();

        for( ; b != e; ++b )
        {
            zpRenderPipelinePass* pass = *b;
            alloc.free( pass );
        }

        m_pipeline.clear();
    }

private:
    zpVector< zpRenderPipelinePass* > m_pipeline;
};

#endif // !ZP_RENDER_PIPELINE_H
