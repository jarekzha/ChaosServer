/*
 * Timer.cpp
 *
 *  Created on: 2014年7月3日
 *  Author: jarek
 *  Brief: 
 */

#include <Time/Timer.h>
#include <Time/TimerManager.h>
#include <Util/TMTSingleton.h>

int Timer::m_runingTimerCount = 0;
bool Timer::m_timerTraceOn = false;
std::map<std::string, int> Timer::m_runningTimerClassMap;

Timer::Timer()
{
    m_timerListNode = NULL;
    m_nextTriggerMSecond = 0;
    m_intervalMSecond = 0;
    m_inTriggerList = false;
}

Timer::~Timer()
{
    if (Going())
    {
        Stop();
    }
}

void Timer::Start(const TimerCallback& callback, UINT intervalMSecond)
{
    ASSERT_RETURN_VOID(!Going());

    TimerManager& timerMgr = TimerManager::Instance();
    if (intervalMSecond < timerMgr.GetTickMSecond())
    {
        intervalMSecond = timerMgr.GetTickMSecond();
    }

    m_callback = callback;
    m_intervalMSecond = intervalMSecond;

    timerMgr.InsertTimer(this);

    m_runingTimerCount++;
    m_runningTimerClassMap[m_callback.GetClassName()]++;
}

void Timer::Stop()
{
    ASSERT_RETURN_VOID(Going());

    m_timerListNode->m_timer = NULL;
    m_timerListNode = NULL;

    if (m_inTriggerList)
    {
        TimerManager::Instance().AbortTimerTrigger(this);
        m_inTriggerList = false;
    }

    m_runingTimerCount--;
    m_runningTimerClassMap[m_callback.GetClassName()]--;
}

void Timer::ChangeIntervalMSecond(UINT newIntervalMSecond)
{
    TimerManager& timerMgr = TimerManager::Instance();
    if (newIntervalMSecond < timerMgr.GetTickMSecond())
    {
        newIntervalMSecond = timerMgr.GetTickMSecond();
    }

    if (m_intervalMSecond == newIntervalMSecond)
    {
        return;
    }

    if (Going())
    {
        Stop();
    }

    Start(m_callback, newIntervalMSecond);
}

void Timer::Trigger()
{
    m_callback.Invoke();
}

UINT Timer::GetIntervalMSecond()
{
    return m_intervalMSecond;
}

UINT Timer::GetIntervalTick()
{
    return m_intervalMSecond / TimerManager::Instance().GetTickMSecond();
}

UINT Timer::GetLeftTick()
{
    ULONG nextTriggerTick = m_nextTriggerMSecond / TimerManager::Instance().GetTickMSecond();
    ULONG lastTriggerTick = TimerManager::Instance().GetLastTriggerTick();
    if (nextTriggerTick <= lastTriggerTick)
    {
        return 1;
    }
    else
    {
        return (nextTriggerTick - lastTriggerTick);
    }
}

void Timer::SetTimerQueueNode(TimerQueueNode* node)
{
    m_timerListNode = node;
}

TimerQueueNode* Timer::GetTimerQueueNode()
{
    return m_timerListNode;
}

ULONG Timer::GetNextTriggerMSecond()
{
    return m_nextTriggerMSecond;
}

void Timer::SetNextTriggerMSecond(ULONG ms)
{
    m_nextTriggerMSecond = ms;
}

void Timer::SetInTriggerList(bool in)
{
    m_inTriggerList = in;
}

bool Timer::Going()
{
    return m_timerListNode != NULL;
}

void Timer::Trace(UINT wheelIndex, UINT absoluteBucketIndex, UINT relativeBucketIndex, UINT wheelCursor)
{
    if (!m_timerTraceOn)
    {
        return;
    }

    TimerTrace trace;
    trace.m_wheelIndex = wheelIndex;
    trace.m_absoluteBucketIndex = absoluteBucketIndex;
    trace.m_relativeBucketIndex = relativeBucketIndex;
    trace.m_wheelCursor = wheelCursor;
    trace.m_leftTick = GetLeftTick();
    trace.m_timerMgrLastTriggerTick = TimerManager::Instance().GetLastTriggerTick();

    m_trace.push_back(trace);

    if (m_trace.size() > 10)
    {
        m_trace.pop_front();
    }
}

int Timer::GetRunningTimerCount()
{
    return m_runingTimerCount;
}

const std::map<std::string, int>& Timer::GetRunningTimerClass()
{
    return m_runningTimerClassMap;
}
