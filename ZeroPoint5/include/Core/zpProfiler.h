#pragma once
#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

#ifndef ZP_USE_PROFILER
#define ZP_PROFILER_BLOCK()         (void)0
#define ZP_PROFILER_START( ... )    (void)0
#define ZP_PROFILER_END( ... )      (void)0
#define ZP_PROFILER_INITIALIZE()    (void)0
#define ZP_PROFILER_FINALIZE()      (void)0
#define ZP_PROFILER_GPU( ... )      (void)0
#else
#define ZP_PROFILER_BLOCK()         zpProfilerBlockS ZP_CONCAT( __profilerBlock, __LINE__)( __FILE__, __FUNCTION__, __LINE__, ZP_NULL )
#define ZP_PROFILER_START( e )      const zp_size_t __profilerIndex##e = g_profiler.start( __FILE__, __FUNCTION__, __LINE__, #e )
#define ZP_PROFILER_END( e )        g_profiler.end( __profilerIndex##e )
#define ZP_PROFILER_INITIALIZE()    g_profiler.initialize()
#define ZP_PROFILER_FINALIZE()      g_profiler.finalize()
#define ZP_PROFILER_GPU( ft, pc )   g_profiler.setRenderingStat( (ft), (pc) )

struct zpProfilerBlockS
{
    zpProfilerBlockS( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName );
    ~zpProfilerBlockS();

    zp_size_t index;
};

enum
{
    ZP_PROFILER_MAX_SAMPLES_PER_FRAME = 1024,
    ZP_PROFILER_MAX_FRAMES = 120
};

struct zpProfilerFrame
{
    const zp_char* fileName;
    const zp_char* functionName;
    const zp_char* eventName;

    zp_long lineNumber;

    zp_size_t threadID;
    zp_size_t parentFrame;

    zp_time_t startTime;
    zp_time_t endTime;

    zp_ulong startCycles;
    zp_ulong endCycles;

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

    zp_ulong frameStartCycles;
    zp_ulong frameEndCycles;

    zp_size_t gpuFrameTime;
    zp_size_t gpuPrimitiveCount;
};

class zpProfiler
{
public:
    static const zp_size_t npos = -1;

    zpProfiler();
    ~zpProfiler();

    zp_size_t start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber, const zp_char* eventName );
    void end( const zp_size_t index );

    void clear();
    void initialize();
    void finalize();

    zp_size_t getCurrentFrame() const;
    zp_size_t getPreviousFrame() const;

    void setRenderingStat( zp_size_t gpuFrameTime, zp_size_t gpuPrimitiveCount );

    const zpProfilerFrameTimeline* getFrameTimelineBegin() const;
    const zpProfilerFrameTimeline* getFrameTimelineEnd() const;

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
