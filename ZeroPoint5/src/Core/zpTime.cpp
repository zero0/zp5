#include "Core/zpCore.h"

#ifdef ZP_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // ZP_WINDOWS

zpTime::zpTime()
    : m_baseTime( 0 )
    , m_currentTime( 0 )
    , m_previousTime( 0 )
    , m_deltaTime( 0 )
    , m_timeSinceStart( 0 )
    , m_wallClockDeltaTime( 0 )
    , m_countsPerSecond( 0 )
    , m_secondsPerTick( 0 )
    , m_timeScale( 0 )
    , m_deltaSeconds( 0 )
    , m_actualDeltaSeconds( 0 )
    , m_interpolation( 0 )
{
#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceFrequency( &t );
    m_countsPerSecond = t.QuadPart;
    m_secondsPerTick = 1.f / static_cast<zp_float>( m_countsPerSecond );

    QueryPerformanceCounter( &t );
    m_baseTime = m_previousTime = t.QuadPart;
#endif // !ZP_WINDOWS
}
zpTime::~zpTime()
{

}

void zpTime::tick()
{
#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    m_currentTime = t.QuadPart;
#endif // !ZP_WINDOWS

    m_wallClockDeltaTime = m_currentTime - m_previousTime;

    m_deltaTime = m_wallClockDeltaTime;
    m_deltaTime = m_deltaTime < 0 ? 0 : m_deltaTime;

    m_previousTime = m_currentTime;

    m_timeSinceStart += m_wallClockDeltaTime;
    m_actualDeltaSeconds = m_deltaTime * m_secondsPerTick;
    m_deltaSeconds = m_actualDeltaSeconds * m_timeScale;
}

void zpTime::reset()
{
#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    m_baseTime = m_previousTime = t.QuadPart;
#endif // !ZP_WINDOWS
}

zp_float zpTime::getDeltaSeconds() const
{
    return m_deltaSeconds;
}

zp_float zpTime::getActualDeltaSeconds() const
{
    return m_actualDeltaSeconds;
}

zp_float zpTime::getSecondsPerTick() const
{
    return m_secondsPerTick;
}

zp_float zpTime::getSecondsSinceStart() const
{
    return m_timeSinceStart * m_secondsPerTick;
}

zp_float zpTime::getWallClockDeltaSeconds() const
{
    return m_wallClockDeltaTime * m_secondsPerTick;
}

void zpTime::setTimeScale( zp_float timeScale )
{
    m_timeScale = timeScale;
}

zp_float zpTime::getTimeScale() const
{
    return m_timeScale;
}

zp_time_t zpTime::getTime() const
{
    zp_time_t time;
#ifdef ZP_WINDOWS
    LARGE_INTEGER t;
    QueryPerformanceCounter( &t );
    time = t.QuadPart;
#endif // !ZP_WINDOWS
    return time;
}

zp_time_t zpTime::getDeltaTime() const
{
    return m_deltaTime;
}

zp_time_t zpTime::getTimeSinceStart() const
{
    return m_timeSinceStart;
}

zp_time_t zpTime::getWallClockDeltaTime() const
{
    return m_wallClockDeltaTime;
}

zp_time_t zpTime::getCountsPerSecond() const
{
    return m_countsPerSecond;
}
