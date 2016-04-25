/*
 * TimingBucket.h
 *
 *  Created on: 2014年7月3日
 *  Author: jarek
 *  Brief: 时间轮中的单个桶
 */

#pragma once

#include <list>

class Timer;
class TimerQueueNode;

class TimingBucket
{
public:
                        TimingBucket();
    virtual             ~TimingBucket();

    void                InsertTimer(Timer* timer);

    void                TakeOutAllTimer(std::list<Timer*>& timerQueue);

    bool                Empty();

private:
    Timer*              Front();

    void                PopFront();

    void                PushBack(Timer* timer);

private:
    TimerQueueNode*      m_timerNodeTail;        // 队列尾节点,头节点为tailNode->nextNode;
};

