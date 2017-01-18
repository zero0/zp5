#pragma once
#ifndef ZP_PROFILER_H
#define ZP_PROFILER_H

#ifndef ZP_USE_PROFILER
#define ZP_PROFILE_BLOCK()      (void)0
#define ZP_PROFILE_START( e )   (void)0
#define ZP_PROFILE_END( e )     (void)0
#define ZP_PROFILE_FINALIZE()   (void)0
#else
#define ZP_PROFILE_BLOCK()      zpProfileBlockS ZP_CONCAT( __profileBlock, __LINE__)( __FILE__, __FUNCTION__, __LINE__ )
#define ZP_PROFILE_START( e )   zp_size_t __profileIndex##e = g_profiler.start( __FILE__, __FUNCTION__, __LINE__ )
#define ZP_PROFILE_END( e )     g_profiler.end( __profileIndex##e )
#define ZP_PROFILE_FINALIZE()   g_profiler.finalize()

struct zpProfileBlockS
{
    zpProfileBlockS( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber );
    ~zpProfileBlockS();

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

    zp_time_t parentFrame;

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
};

class zpProfiler
{
public:
    static const zp_size_t npos = -1;

    zpProfiler();
    ~zpProfiler();

    zp_size_t start( const zp_char* fileName, const zp_char* functionName, zp_long lineNumber );
    void end( zp_size_t index );

    void clear();
    void finalize();

    const zpProfilerFrame* getPreviousFrameBegin() const;
    const zpProfilerFrame* getPreviousFrameEnd() const;

private:
    zp_size_t m_currentFrame;
    zp_size_t m_previousFrame;

    zpProfilerFrameTimeline m_timelines[ ZP_PROFILER_MAX_FRAMES ];
};


static zpProfiler g_profiler;

#endif

#endif // !ZP_PROFILER_H
