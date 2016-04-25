/*
 * TimerManager.cpp
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 
 */

#include <Time/Time.h>
#include <Time/Timer.h>
#include <Time/TimerManager.h>
#include <cmath>

using namespace std;

TimerManager::TimerManager()
{
    m_tickMSecond = 0;
    m_timerRangeMSecond = 0;
    m_lastTriggerTick = 0;
}


TimerManager::~TimerManager()
{
    // NULL
}

bool TimerManager::Initialize(UINT tickMSecond, UINT wheelNum, UINT buckNumPerWheel)
{
    m_tickMSecond = tickMSecond;
    m_timerRangeMSecond = tickMSecond * pow(buckNumPerWheel, wheelNum);
    m_lastTriggerTick = Time::GetCurMSecond() / tickMSecond;

    m_wheel.resize(wheelNum);
    for (TimingWheelVector::size_type i = 0; i < m_wheel.size(); ++i)
    {
        if (!m_wheel[i].Initialize(i, buckNumPerWheel))
        {
            return false;
        }
    }

    return true;
}


void TimerManager::Run()
{
    // 两次运行时间不够一个tick
    ULONG curTick = Time::GetCurMSecond() / m_tickMSecond;
    if (curTick <= m_lastTriggerTick)
    {
        return;
    }

    UINT runTickNum = curTick - m_lastTriggerTick;
    for (UINT i = 0; i < runTickNum; ++i)
    {
        m_timerTriggerList.clear();

        // 执行一个tick，并获取到时间的timer
        {
            m_lastTriggerTick++;

            // 取出最小轮cursor所指向的bucket中的timer
            m_wheel[0].TakeOutCursorBucketAllTimer(m_timerTriggerList);

            DoOneTick();

            for (TimerTriggerList::iterator iter = m_timerTriggerList.begin(); iter != m_timerTriggerList.end(); ++iter)
            {
                Timer* timer = *iter;
                ASSERT_CONTINUE(timer != NULL);

                timer->SetInTriggerList(true);
                NoneMutexAddInsertTimer(timer);
            }
        }

        // 统一触发timer
        while (!m_timerTriggerList.empty())
        {
            Timer* timer = NULL;

            {
                timer = m_timerTriggerList.front();
                m_timerTriggerList.pop_front();
            }

            ASSERT_CONTINUE(timer != NULL);
            timer->SetInTriggerList(false);
            timer->Trigger();

        }
    }
}

void TimerManager::InsertTimer(Timer* timer)
{
    NoneMutexAddInsertTimer(timer);
}

UINT TimerManager::GetTickMSecond()
{
    return m_tickMSecond;
}

UINT TimerManager::GetTimerRangeMSecond()
{
    return m_timerRangeMSecond;
}

ULONG TimerManager::GetLastTriggerTick()
{
    return m_lastTriggerTick;
}

void TimerManager::AbortTimerTrigger(Timer* timer)
{
    ASSERT_RETURN_VOID(timer != NULL);


    for (TimerTriggerList::iterator iter = m_timerTriggerList.begin();
            iter != m_timerTriggerList.end(); ++iter)
    {
        Timer* tempTimer = *iter;
        if (tempTimer == timer)
        {
            m_timerTriggerList.erase(iter);
            break;
        }
    }
}

UINT TimerManager::CalcTimerWheelIndex(UINT intervalTick)
{
    for (TimingWheelVector::size_type i = m_wheel.size() - 1; i > 0; i--)
    {
        if (m_wheel[i].GetMaxTickNum() >= intervalTick
                && m_wheel[i - 1].GetMaxTickNum() < intervalTick)
        {
            return i;
        }
    }

    return 0;
}

void TimerManager::DoOneTick()
{
    for (TimingWheelVector::size_type i = 0; i < m_wheel.size(); ++i)
    {
        if (!m_wheel[i].DoOneTick())
        {
            break;
        }
    }
}

void TimerManager::NoneMutexAddInsertTimer(Timer* timer)
{
    ASSERT_RETURN_VOID(timer != NULL);
    ASSERT_RETURN_VOID(timer->GetIntervalMSecond() <= GetTimerRangeMSecond());

    timer->SetNextTriggerMSecond(Time::GetCurMSecond() + timer->GetIntervalMSecond());

    UINT leftTick = timer->GetLeftTick();
    UINT wheelIndex = CalcTimerWheelIndex(leftTick);

    m_wheel[wheelIndex].InsertTimer(timer);
}

void TimerManager::NoneMutexChangeInsertTimer(list<Timer*>& timerList)
{
    for (list<Timer*>::iterator iter = timerList.begin(); iter != timerList.end(); ++iter)
    {
        Timer* timer = *iter;
        ASSERT_CONTINUE(timer != NULL);

        UINT leftTick = timer->GetLeftTick();
        UINT wheelIndex = CalcTimerWheelIndex(leftTick);

        m_wheel[wheelIndex].InsertTimer(timer);
    }
}
