/*
 * TimingWheel.cpp
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 
 */

#include <Time/Timer.h>
#include <Time/TimingWheel.h>
#include <Time/TimerManager.h>
#include <cmath>
#include <queue>

using namespace std;

TimingWheel::TimingWheel()
{
    m_cursor = 0;
    m_index = 0;
    m_maxTickNum = 0;
    m_minTickNum = 0;
    m_bucketUnitOfTickNum = 0;
}

TimingWheel::~TimingWheel()
{
    // NULL;
}

bool TimingWheel::Initialize(UINT index, UINT bucketNum)
{
    m_bucket.resize(bucketNum);

    m_cursor = 0;
    m_index = index;
    m_maxTickNum = pow(bucketNum, index + 1);
    if (index == 0)
    {
        m_minTickNum = 1;
    }
    else
    {
        m_minTickNum = pow(bucketNum, index) + 1;
    }
    m_bucketUnitOfTickNum = pow(bucketNum, index);

    return true;
}

void TimingWheel::InsertTimer(Timer* timer)
{
    UINT leftTick = timer->GetLeftTick();
    assert(leftTick >= m_minTickNum && leftTick <= m_maxTickNum);

    int bucketOffsetIndex = int(leftTick - m_minTickNum) / m_bucketUnitOfTickNum;
    UINT bucketIndex = (m_cursor + bucketOffsetIndex) % m_bucket.size();

    m_bucket[bucketIndex].InsertTimer(timer);

    timer->Trace(m_index, bucketIndex, bucketOffsetIndex, m_cursor);
}

void TimingWheel::TakeOutCursorBucketAllTimer(std::list<Timer*>& timerQueue)
{
    m_bucket[m_cursor].TakeOutAllTimer(timerQueue);
}

bool TimingWheel::DoOneTick()
{
    UINT formerCursor = m_cursor;

    // 移动光标
    m_cursor++;

    // 进位
    bool carry = false;     // 是否进位
    if (m_cursor >= m_bucket.size())
    {
        m_cursor = 0;
        carry = true;
    }

    // 当前指向的bucket里的timer放到低位wheel
    list<Timer*> timerList;
    m_bucket[formerCursor].TakeOutAllTimer(timerList);
    TimerManager& timerMgr = TimerManager::Instance();
    if (!timerList.empty())
    {
        timerMgr.NoneMutexChangeInsertTimer(timerList);
    }

    return carry;
}

UINT TimingWheel::GetMaxTickNum()
{
    return m_maxTickNum;
}

UINT TimingWheel::GetBucketUnitOfTickNum()
{
    return m_bucketUnitOfTickNum;
}
