/*
 * TimingBucket.cpp
 *
 *  Created on: 2014年7月3日
 *  Author: jarek
 *  Brief: 
 */

#include <Time/Timer.h>
#include <Time/TimingBucket.h>
#include <Util/Util.h>

TimingBucket::TimingBucket()
{
    m_timerNodeTail = NULL;
}

TimingBucket::~TimingBucket()
{
    while (!Empty())
    {
        PopFront();
    }
}

void TimingBucket::InsertTimer(Timer* timer)
{
    ASSERT_RETURN_VOID(timer != NULL || timer->GetTimerQueueNode() != NULL);

    PushBack(timer);
}

void TimingBucket::TakeOutAllTimer(std::list<Timer*>& timerQueue)
{
    while (!Empty())
    {
        Timer* timer = Front();
        if (timer != NULL)
        {
            timerQueue.push_back(timer);
        }

        PopFront();
    }
}

Timer* TimingBucket::Front()
{
    if (NULL == m_timerNodeTail)
    {
        return NULL;
    }

    return m_timerNodeTail->m_nextNode->m_timer;
}

void TimingBucket::PopFront()
{
    ASSERT_RETURN_VOID(m_timerNodeTail != NULL);

    TimerQueueNode* frontNode = m_timerNodeTail->m_nextNode;

    // 只有一个节点
    if (m_timerNodeTail == frontNode)
    {
        m_timerNodeTail = NULL;
    }
    else
    {
        m_timerNodeTail->m_nextNode = frontNode->m_nextNode;
    }

    // 移除挂接关系
    if (frontNode->m_timer != NULL)
    {
        frontNode->m_timer->SetTimerQueueNode(NULL);
    }

    delete frontNode;
}

void TimingBucket::PushBack(Timer* timer)
{
    TimerQueueNode* newNode = new TimerQueueNode;
    newNode->m_nextNode = m_timerNodeTail;
    newNode->m_timer = timer;

    if (m_timerNodeTail == NULL)
    {
        newNode->m_nextNode = newNode;
        m_timerNodeTail = newNode;
    }
    else
    {
        newNode->m_nextNode = m_timerNodeTail->m_nextNode;
        m_timerNodeTail->m_nextNode = newNode;
        m_timerNodeTail = newNode;
    }

    // 挂接timer
    timer->SetTimerQueueNode(newNode);
}

bool TimingBucket::Empty()
{
    return m_timerNodeTail == NULL;
}
