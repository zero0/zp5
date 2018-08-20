#pragma once
#ifndef ZP_RENDER_PIPELINE_H
#define ZP_RENDER_PIPELINE_H

ZP_ABSTRACT_CLASS zpRenderPipelinePass
{
public:
    zpRenderPipelinePass();
    virtual ~zpRenderPipelinePass();

    void update();

    void executePass( const zpCamera* camera, zpRenderCommandBuffer* cmdBuffer );

protected:
    virtual void onUpdate()
    {
    };

    virtual void onRender( const zpCamera* camera, zpRenderCommandBuffer* cmdBuffer )
    {
    };

private:
};

template<typename Allocator = zpDefaultGlobalAllocator>
class zpRenderPipelinePassFactory
{
    ZP_STATIC_CLASS( zpRenderPipelinePassFactory );
public:
    typedef Allocator allocator_value;

    template<typename T>
    static T* createRenderPipelinePass()
    {
        const void* pass = s_allocator.allocate( sizeof( T ) );
        return static_cast<T>( pass );
    }

    template<typename T>
    static void destroyRenderPipelinePass( T* pass )
    {
        s_allocator.free( pass );
    }

private:
    static allocator_value s_allocator;
};

//template<typename Allocator>
//static Allocator zpRenderPipelinePassFactory<Allocator>::s_allocator;

class zpRenderPipeline
{
public:
    zpRenderPipeline();
    ~zpRenderPipeline();

    void addPass( zpRenderPipelinePass* pass );

    void executePipeline( const zpCamera* camera, zpRenderCommandBuffer* cmdBuffer );

    void clear();

private:
    zpVector< zpRenderPipelinePass* > m_pipeline;
};

#endif // !ZP_RENDER_PIPELINE_H
