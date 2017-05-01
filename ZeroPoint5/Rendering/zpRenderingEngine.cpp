#include "zpRendering.h"

#ifdef ZP_USE_OPENGL_RENDERING
#include "RenderingOpenGL\zpRenderingOpenGL.h"
#endif

static ZP_FORCE_INLINE void SkipAnchors( zpSortRenderCommandData* &start, zpSortRenderCommandData* end )
{
    for( ; start != end && start->key.anchor; ++start )
    {
    }
}

static ZP_FORCE_INLINE void SkipToNextAnchor( zpSortRenderCommandData* &start, zpSortRenderCommandData* end )
{
    for( ; start != end && !start->key.anchor; ++start )
    {
    }
}

static void SortRenderCommands( const zpVector<zpRenderingCommand>& commands, zp_byte viewport, zpVector<zpSortRenderCommandData>& sort )
{
    ZP_PROFILER_BLOCK();

    sort.reset();
    sort.reserve( commands.size() );

    for( zp_size_t i = 0, imax = commands.size(); i < imax; ++i )
    {
        const zpRenderingCommand& cmd = commands[ i ];
        if( cmd.sortKey.viewport == viewport )
        {
            zpSortRenderCommandData& data = sort.pushBackEmpty();
            data.key = cmd.sortKey;
            data.index = i;
        }
    }

    zpSortRenderCommandData* begin = sort.begin();
    zpSortRenderCommandData* end = sort.end();

    zpSortRenderCommandData* sortStart = begin;
    zpSortRenderCommandData* sortEnd = begin;

    do
    {
        sortStart = sortEnd;

        SkipAnchors( sortStart, end );

        sortEnd = sortStart;

        SkipToNextAnchor( sortEnd, end );

        if( sortStart == sortEnd )
        {
            break;
        }

        // sort by layers
        zp_qsort( sortStart, sortEnd, []( zpSortRenderCommandData& a, zpSortRenderCommandData& b )
        {
            return a.key.layer < b.key.layer;
        } );
        
        // sort depth of each layer, reversing sort for transparent layers
        zp_qsort( sortStart, sortEnd, []( zpSortRenderCommandData& a, zpSortRenderCommandData& b )
        {
            if( a.key.layer == b.key.layer )
            {
                if( a.key.layer & 0x7 )
                {
                    return a.key.depth > b.key.depth;
                }
                else
                {
                    return a.key.depth < b.key.depth;
                }
            }
            return a.key.layer < b.key.layer;
        } );

        // sort materials
        zp_qsort( sortStart, sortEnd, []( zpSortRenderCommandData& a, zpSortRenderCommandData& b )
        {
            return a.key.material < b.key.material;
        } );

        // sort passes
        zp_qsort( sortStart, sortEnd, []( zpSortRenderCommandData& a, zpSortRenderCommandData& b )
        {
            return a.key.pass < b.key.pass;
        } );
    } while( sortStart != sortEnd );
}

zpRenderingEngine::zpRenderingEngine()
{

}
zpRenderingEngine::~zpRenderingEngine()
{

}

void zpRenderingEngine::setup( zp_handle hWindow )
{
    ZP_PROFILER_BLOCK();

    //m_engine.setup( hWindow );
    SetupRenderingOpenGL( hWindow, m_hDC, m_hContext );

    m_immidiateContext.setup();
}
void zpRenderingEngine::teardown()
{
    ZP_PROFILER_BLOCK();
    
    m_immidiateContext.teardown();

    //m_engine.teardown();
    TeardownRenderingOpenGL( m_hContext );

    m_hDC = ZP_NULL;
    m_hContext = ZP_NULL;
}

void zpRenderingEngine::setWindowSize( const zpVector2i& size )
{
    //m_engine.setWindowSize( size );
}

void zpRenderingEngine::present()
{
    ZP_PROFILER_BLOCK();
    
    zpRenderingStat stat;

    BeginFrameOpenGL();

    m_immidiateContext.fillBuffers();

    renderingPassForward( m_immidiateContext );

    m_immidiateContext.flipBuffers();

    EndFrameOpenGL( stat );

    PresentOpenGL( m_hDC, m_hContext );

    ZP_PROFILER_GPU( stat.frameTime, stat.primitiveCount );
}

zp_bool zpRenderingEngine::isVSync() const
{
    return m_isVSync;
}
void zpRenderingEngine::setVSync( zp_bool vsync )
{
    m_isVSync = vsync;
}

zpRenderingContext* zpRenderingEngine::getImmidiateContext()
{
    return &m_immidiateContext;
}

void zpRenderingEngine::createRenderBuffer( zpBufferType type, zpBufferBindType bindType, zp_size_t size, const void* data, zpRenderBuffer& buffer )
{
    ZP_PROFILER_BLOCK();
    
    CreateRenderBufferOpenGL( type, bindType, size, data, buffer );
}

void zpRenderingEngine::destoryRenderBuffer( zpRenderBuffer& buffer )
{
    ZP_PROFILER_BLOCK();

    DestroyRenderBufferOpenGL( buffer );
}

void zpRenderingEngine::createTexture( zp_uint width, zp_uint height, zp_int mipMapCount, zpDisplayFormat displayFormat, zpTextureDimension textureDimension, zpTextureType textureType, const void* pixels, zpTexture& texture )
{
    ZP_PROFILER_BLOCK();

    CreateTextureOpenGL( width, height, mipMapCount, displayFormat, textureDimension, textureType, pixels, texture );
}

void zpRenderingEngine::destroyTexture( zpTexture& texture )
{
    ZP_PROFILER_BLOCK();

    DestroyTextureOpenGL( texture );
}

void zpRenderingEngine::createShader( const zp_char* vertexShaderSource, const zp_char* fragmentShaderSource, zpShader& shader )
{
    ZP_PROFILER_BLOCK();

    CreateShaderOpenGL( vertexShaderSource, fragmentShaderSource, shader );
}

void zpRenderingEngine::destroyShader( zpShader& shader )
{
    ZP_PROFILER_BLOCK();
   
    DestroyShaderOpenGL( shader );
}

void zpRenderingEngine::renderingPassForward( zpRenderingContext& context )
{
    ZP_PROFILER_BLOCK();
    
    const zpVector< zpRenderingCommand >& cmds = context.getCommands();

    m_sortedCommands.reset();
    SortRenderCommands( cmds, 0, m_sortedCommands );

    for( zpSortRenderCommandData* b = m_sortedCommands.begin(), *e = m_sortedCommands.end(); b != e; ++b )
    {
        const zpRenderingCommand* cmd = cmds.begin() + b->index;
        ProcessRenderingCommandOpenGL( cmd );
    }
}
