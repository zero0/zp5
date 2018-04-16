#pragma once
#ifndef ZP_TIME_H
#define ZP_TIME_H

class zpTime
{
private:
    zpTime();
    ~zpTime();

public:
    static zpTime& get();

    void tick();
    void reset();

    zp_float getDeltaSeconds() const;
    zp_float getActualDeltaSeconds() const;
    zp_float getSecondsPerTick() const;
    zp_float getSecondsSinceStart() const;
    zp_float getWallClockDeltaSeconds() const;

    void setTimeScale( zp_float timeScale );
    zp_float getTimeScale() const;

    zp_size_t getFrameCount() const;

    zp_time_t getTime() const;
    zp_time_t getDeltaTime() const;
    zp_time_t getTimeSinceStart() const;
    zp_time_t getWallClockDeltaTime() const;
    zp_time_t getCountsPerSecond() const;

private:
    zp_time_t m_baseTime;
    zp_time_t m_currentTime;
    zp_time_t m_previousTime;
    zp_time_t m_deltaTime;
    zp_time_t m_timeSinceStart;
    zp_time_t m_wallClockDeltaTime;
    zp_time_t m_countsPerSecond;

    zp_size_t m_frameCount;

    zp_float m_secondsPerTick;
    zp_float m_timeScale;
    zp_float m_deltaSeconds;
    zp_float m_actualDeltaSeconds;
    zp_float m_interpolation;
};

#endif // !ZP_TIME_H
