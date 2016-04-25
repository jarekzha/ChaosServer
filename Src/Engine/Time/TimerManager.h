/*
 * TimerManager.h
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 定时器管理器
 */

#pragma once

#include <Thread/ThreadMutex.h>
#include <Time/TimingWheel.h>
#include <Util/TMTSingleton.h>
#include <Util/Util.h>
#include <vector>
#include <list>

static const int TIMER_MS_PER_TICK = 100; // 每100ms一个tick，也就是10帧每秒
static const int TIMER_WHELL_NUM = 4; // 4个时间轮
static const int TIMER_BUCKET_PER_WHELL = 100; // 每个时间轮100个bucket

class Timer;

class TimerManager: public TMTSingleton<TimerManager>
{
public:
    TimerManager();
    virtual             ~TimerManager();

    bool                Initialize(UINT tickMSecond = TIMER_MS_PER_TICK, UINT wheelNum = TIMER_WHELL_NUM,
                            UINT buckNumPerWheel = TIMER_BUCKET_PER_WHELL);

    void                Run();

    void                InsertTimer(Timer* timer);

    UINT                GetTickMSecond();

    UINT                GetTimerRangeMSecond();

    ULONG               GetLastTriggerTick();

    void                AbortTimerTrigger(Timer* timer);

private:
    UINT                CalcTimerWheelIndex(UINT intervalTick);

    void                DoOneTick();

    void                NoneMutexAddInsertTimer(Timer* timer);

    void                NoneMutexChangeInsertTimer(std::list<Timer*>& timerList);

private:
    typedef std::vector<TimingWheel> TimingWheelVector;
    typedef std::list<Timer*> TimerTriggerList;

    TimingWheelVector   m_wheel;                // 时间轮子
    UINT                m_tickMSecond;          // 单个tick的毫秒数，timer的最小精度
    UINT                m_timerRangeMSecond;    // 定时器范围（毫秒）
    ULONG               m_lastTriggerTick;      // 上次触发时的tick数，从毫秒数转换而来
    TimerTriggerList    m_timerTriggerList;     // timer触发列表

    friend class        TimingWheel;
};
