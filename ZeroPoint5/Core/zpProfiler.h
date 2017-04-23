#pragma once
#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

#ifndef ZP_USE_PROFILER
#define ZP_PROFILER_BLOCK()      (void)0
#define ZP_PROFILER_START( e )   (void)0
#define ZP_PROFILER_END( e )     (void)0
#define ZP_PROFILER_FINALIZE()   (void)0
#else
#define ZP_PROFILER_BLOCK()      zpProfilerBlockS ZP_CONCAT( __profilerBlock, __LINE__)( __FILE__, __FUNCTION__, __LINE__, ZP_NULL )
#define ZP_PROFILER_START( e )   zp_size_t __profilerIndex##e = g_profiler.start( __FILE__, __FUNCTION__, __LINE__, #e )
#define ZP_PROFILER_END( e )     g_profiler.end( __profilerIndex##e )
#define ZP_PROFILER_FINALIZE()   g_profiler.finalize()

struct zpProfilerBlockS
{
    zpProfilerBlockS( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName );
    ~zpProfilerBlockS();

    zp_size_t index;
};

enum
{
    ZP_PROFILER_MAX_SAMPLES_PER_FRAME = 128,
    ZP_PROFILER_MAX_FRAMES = 120
};

struct zpProfilerFrame
{
    const zp_char* fileName;
    const zp_char* functionName;
    zp_long lineNumber;
    const zp_char* eventName;

    zp_size_t parentFrame;

    zp_time_t startTime;
    zp_time_t endTime;

    zp_size_t startMemory;
    zp_size_t endMemory;
};

struct zpProfilerFrameTimeline
{
    zpProfilerFrame frames[ ZP_PROFILER_MAX_SAMPLES_PER_FRAME ];
    zp_size_t frameStack[ ZP_PROFILER_MAX_SAMPLES_PER_FRAME ];
    zp_size_t size;
    zp_size_t stackSize;
    zp_time_t frameStartTime;
    zp_time_t frameEndTime;
};

class zpProfiler
{
public:
    static const zp_size_t npos = -1;

    zpProfiler();
    ~zpProfiler();

    zp_size_t start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName );
    void end( zp_size_t index );

    void clear();
    void finalize();

    const zpProfilerFrameTimeline* getPreviousFrameTimeline() const;
    const zpProfilerFrame* getPreviousFrameBegin() const;
    const zpProfilerFrame* getPreviousFrameEnd() const;

private:
    zp_size_t m_currentFrame;
    zp_size_t m_previousFrame;

    zpProfilerFrameTimeline m_timelines[ ZP_PROFILER_MAX_FRAMES ];
};


extern zpProfiler g_profiler;

#endif

#endif // !ZP_PROFILER_H
