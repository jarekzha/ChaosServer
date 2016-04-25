/*
 * RecoverTimer.cpp
 *
 *  Created on: 2014年9月26日
 *      Author: kcinz
 */

#include <Time/Time.h>
#include <iostream>
#include "RecoverTimer.h"

using namespace std;

RecoverTimer::RecoverTimer(RealFunction realFunction, RecoverAfterLoad afterLoad)
{
    m_loadTimeMs = 0;
    m_lastTimeMs = 0;
    m_realIntervalMs = 0;

    m_realFunction = realFunction;
    m_recoverAfterLoad = afterLoad;

    m_isLoadResume = true;      // 默认在运行过RecoverAfterLoad之后是需要继续的
}

RecoverTimer::~RecoverTimer()
{

}

void RecoverTimer::Start(ULONG intervalMSecond)
{
    m_realIntervalMs = intervalMSecond;
    m_lastTimeMs = Time::GetCurMSecond();
    m_timer.Start(TimerCallback(this, &RecoverTimer::RealTimerFunction), m_realIntervalMs);

#ifdef SHOW_DEBUG_LOG
    cout << "Begin Timer, This Interval time = " << m_realIntervalMs << "ms" << endl;
#endif
}

void RecoverTimer::Stop()
{
    m_loadTimeMs = 0;
    m_lastTimeMs = 0;
    m_realIntervalMs = 0;

    if (m_timer.Going()) // 如果timer已经启动，这时需要先停止
        m_timer.Stop();
}

void RecoverTimer::RealTimerFunction()
{
    m_realFunction();   // 触发一次
    m_lastTimeMs = Time::GetCurMSecond();   // 记录每个上一次触发时间

#ifdef SHOW_DEBUG_LOG
    cout << "lastTimeMs = " << m_lastTimeMs << endl;
#endif
}

void RecoverTimer::RemainTimerFunction()
{
    //m_realFunction();
    RealTimerFunction();
    if (m_timer.Going()) // 如果timer已经启动，这时需要先停止
        m_timer.Stop();

    if (false == m_isLoadResume) // 如果不是无限循环的timer，则不再继续
        return;

    m_timer.Start(TimerCallback(this, &RecoverTimer::RealTimerFunction), m_realIntervalMs);

#ifdef SHOW_DEBUG_LOG
    cout << "Recover Timer, This Interval time = " << m_realIntervalMs << "ms" << endl;
#endif
}

bool RecoverTimer::LoadPostPorcess()
{
    // 间隔时间为0，则直接返回，说明timer还未开始启用
    if (0 == m_realIntervalMs)
        return true;

    m_loadTimeMs = Time::GetCurMSecond();

    ULONG passTimeMs = m_loadTimeMs - m_lastTimeMs;
    int count = passTimeMs / m_realIntervalMs;

    //计算最近一次触发的时间
    if(count > 0)
    {
        m_lastTimeMs += (count * m_realIntervalMs);
    }

#ifdef SHOW_DEBUG_LOG
    cout << "lastTimeMs = " << m_lastTimeMs << endl;
    cout << "loadTimeMs = " << m_loadTimeMs << endl;
    cout << "passTimeMs = " << passTimeMs << endl;
    cout << "realInterval = " << m_realIntervalMs << endl;
#endif

    ULONG remainTimeMs = m_realIntervalMs - passTimeMs % m_realIntervalMs;
    if (false == m_isLoadResume && count > 0)   // 如果不是无限循环的timer，则立刻设置退出时间为1ms，尽快退出
    {
        m_recoverAfterLoad(count);
        return true;
    }

    if (m_timer.Going()) // 如果timer已经启动，这时需要先停止
        m_timer.Stop();
    m_timer.Start(TimerCallback(this, &RecoverTimer::RemainTimerFunction), remainTimeMs);

    m_recoverAfterLoad(count);

#ifdef SHOW_DEBUG_LOG
    cout << "Remain Timer, This Interval time = " << remainTimeMs << "ms" << endl;
#endif
    return true;
}

ULONG RecoverTimer::GetNextTimePoint()
{
    return m_lastTimeMs + m_realIntervalMs;
}

void RecoverTimer::SetIsLoadResume(bool isResume)
{
    m_isLoadResume = isResume;
}
