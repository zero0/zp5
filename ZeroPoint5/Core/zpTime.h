#pragma once
#ifndef ZP_TIME_H
#define ZP_TIME_H

class zpTime
{
public:
    zpTime();
    ~zpTime();

    void tick();
    void reset();

    zp_float getDeltaSeconds() const;
    zp_float getActualDeltaSeconds() const;
    zp_float getSecondsPerTick() const;
    zp_float getSecondsSinceStart() const;
    zp_float getWallClockDeltaSeconds() const;

    void setTimeScale( zp_float timeScale );
    zp_float getTimeScale() const;

    zp_long getTime() const;
    zp_long getDeltaTime() const;
    zp_long getTimeSinceStart() const;
    zp_long getWallClockDeltaTime() const;
    zp_long getCountsPerSecond() const;

private:
    zp_long m_baseTime;
    zp_long m_currentTime;
    zp_long m_previousTime;
    zp_long m_deltaTime;
    zp_long m_timeSinceStart;
    zp_long m_wallClockDeltaTime;
    zp_long m_countsPerSecond;

    zp_float m_secondsPerTick;
    zp_float m_timeScale;
    zp_float m_deltaSeconds;
    zp_float m_actualDeltaSeconds;
    zp_float m_interpolation;
};

#endif // !ZP_TIME_H
