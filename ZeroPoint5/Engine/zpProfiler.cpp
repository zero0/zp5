#include "zpEngine.h"

#ifdef ZP_USE_PROFILER

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

zpProfileBlockS::zpProfileBlockS( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber )
    : index( g_profiler.start( fileName, functionName, lineNumber ) )
{
}

zpProfileBlockS::~zpProfileBlockS()
{
    g_profiler.end( index );
}

ZP_FORCE_INLINE zp_time_t GetTime()
{
    zp_time_t time;

#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    time = t.QuadPart;
#endif // !ZP_WINDOWS

    return time;
}

zpProfiler::zpProfiler()
    : m_currentFrame( 0 )
    , m_previousFrame( 0 )
{
    clear();
}

zpProfiler::~zpProfiler()
{
    clear();
}

zp_size_t zpProfiler::start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber )
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    zp_size_t index = t->size;
    zp_size_t parentFrame = npos;

    if( t->stackSize > 0 )
    {
        parentFrame = t->frameStack[ t->stackSize - 1 ];
    }

    t->frameStack[ t->stackSize ] = index;

    ++t->size;
    ++t->stackSize;

    zpProfilerFrame* frame = t->frames + index;
    frame->fileName = fileName;
    frame->functionName = functionName;
    frame->lineNumber = lineNumber;

    frame->parentFrame = parentFrame;

    frame->startTime = GetTime();
    frame->endTime = 0;

    frame->startMemory = 0;
    frame->endMemory = 0;

    return index;
}

void zpProfiler::end( zp_size_t index )
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    zpProfilerFrame* frame = t->frames + index;

    frame->endTime = GetTime();
    frame->endMemory = 0;

    if( t->stackSize > 0 )
    {
        --t->stackSize;
    }
}

void zpProfiler::clear()
{
    zp_memset( m_timelines, 0, sizeof( m_timelines ) );
}

void zpProfiler::finalize()
{
    m_previousFrame = m_currentFrame;
    m_currentFrame = ( m_currentFrame + 1 ) % ZP_PROFILER_MAX_FRAMES;
    
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    t->size = 0;
    t->stackSize = 0;
}

const zpProfilerFrame* zpProfiler::getPreviousFrameBegin() const
{
    const zpProfilerFrameTimeline* t = m_timelines + m_previousFrame;
    return t->frames;
}

const zpProfilerFrame* zpProfiler::getPreviousFrameEnd() const
{
    const zpProfilerFrameTimeline* t = m_timelines + m_previousFrame;
    return t->frames + t->size;
}

#endif
