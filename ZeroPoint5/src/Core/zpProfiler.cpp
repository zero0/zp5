#include "Core/zpCore.h"

#ifdef ZP_USE_PROFILER

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

zpProfilerBlockS::zpProfilerBlockS( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName )
    : index( g_profiler.start( fileName, functionName, lineNumber, eventName ) )
{
}

zpProfilerBlockS::~zpProfilerBlockS()
{
    g_profiler.end( index );
}

ZP_FORCE_INLINE static zp_time_t GetTime()
{
    zp_time_t time;

#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    time = t.QuadPart;
#endif // !ZP_WINDOWS

    return time;
}

ZP_FORCE_INLINE static zp_ulong GetCycles()
{
    zp_ulong cycles;
#ifdef ZP_WINDOWS
    cycles = __rdtsc();
#endif // !ZP_WINDOWS
    return cycles;
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

zp_size_t zpProfiler::start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName )
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
    frame->eventName = eventName;

    frame->threadID = 0;
    frame->parentFrame = parentFrame;

    frame->startTime = GetTime();
    frame->endTime = 0;
    
    frame->startCycles = GetCycles();
    frame->endCycles = 0;

    frame->startMemory = g_globalAllocator->getMemoryUsed();
    frame->endMemory = 0;

    return index;
}

void zpProfiler::end( const zp_size_t index )
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    zpProfilerFrame* frame = t->frames + index;

    frame->endTime = GetTime();
    frame->endCycles = GetCycles();
    frame->endMemory = g_globalAllocator->getMemoryUsed();

    if( t->stackSize > 0 )
    {
        --t->stackSize;
    }
}

void zpProfiler::clear()
{
    m_currentFrame = 0;
    m_previousFrame = 0;
    zp_memset( m_timelines, 0, sizeof( m_timelines ) );
}

void zpProfiler::initialize()
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    t->size = 0;
    t->stackSize = 0;
    t->frameStartTime = GetTime();
    t->frameStartCycles = GetCycles();
    t->frameStartMemory = g_globalAllocator->getMemoryUsed();
}

void zpProfiler::finalize()
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    t->frameEndTime = GetTime();
    t->frameEndCycles = GetCycles();
    t->frameEndMemory = g_globalAllocator->getMemoryUsed();

    m_previousFrame = m_currentFrame;
    m_currentFrame = ( m_currentFrame + 1 ) % ZP_PROFILER_MAX_FRAMES;
}

zp_size_t zpProfiler::getCurrentFrame() const
{
    return m_currentFrame;
}

zp_size_t zpProfiler::getPreviousFrame() const
{
    return m_previousFrame;
}

void zpProfiler::setRenderingStat( zp_size_t gpuFrameTime, zp_size_t gpuPrimitiveCount )
{
    zpProfilerFrameTimeline* t = m_timelines + m_currentFrame;
    t->gpuFrameTime = gpuFrameTime;
    t->gpuPrimitiveCount = gpuPrimitiveCount;
}


const zpProfilerFrameTimeline* zpProfiler::getFrameTimelineBegin() const
{
    return m_timelines;
}

const zpProfilerFrameTimeline* zpProfiler::getFrameTimelineEnd() const
{
    return m_timelines + ZP_PROFILER_MAX_FRAMES;
}

const zpProfilerFrameTimeline* zpProfiler::getPreviousFrameTimeline() const
{
    const zpProfilerFrameTimeline* t = m_timelines + m_previousFrame;
    return t;
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
