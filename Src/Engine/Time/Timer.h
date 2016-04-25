/*
 * Timer.h
 *
 *  Created on: 2014年7月3日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Time/TimerCallback.h>
#include <Util/Util.h>
#include <list>
#include <string>
#include <map>

// 定时器时间单位
#define TIMER_SECOND    1000
#define TIMER_MINUTE    (60 * TIMER_SECOND)
#define TIMER_HOUR      (60 * TIMER_MINUTE)
#define TIMER_DAY       (24 * TIMER_HOUR)

class Timer;

// timer列表中的节点信息
struct TimerQueueNode
{
    TimerQueueNode*     m_nextNode;
    Timer*              m_timer;
};

// timer调试
struct TimerTrace
{
    UINT                m_wheelIndex;
    UINT                m_absoluteBucketIndex;
    UINT                m_relativeBucketIndex;
    UINT                m_wheelCursor;
    UINT                m_leftTick;
    UINT                m_timerMgrLastTriggerTick;
};

class TimingBucket;

/*
 * 定时器
 * 调用Start开启后，会一直保持预设频率触发，直到Stop或者被删除
 */
class Timer
{
public:
                        Timer();
    virtual             ~Timer();

    void                Start(const TimerCallback& callback, UINT intervalMSecond);

    void                Stop();

    void                ChangeIntervalMSecond(UINT newIntervalMSecond);

    void                Trigger();

    UINT                GetIntervalMSecond();

    UINT                GetIntervalTick();
    UINT                GetLeftTick();

    void                SetTimerQueueNode(TimerQueueNode* node);
    TimerQueueNode*     GetTimerQueueNode();

    ULONG               GetNextTriggerMSecond();
    void                SetNextTriggerMSecond(ULONG ms);

    // 设置正在触发中
    void                SetInTriggerList(bool in);

    bool                Going();

    void                Trace(UINT wheelIndex, UINT absoluteBucketIndex, UINT relativeBucketIndex, UINT wheelCursor);

public:
    static int          GetRunningTimerCount();
    static const std::map<std::string, int>& GetRunningTimerClass();

private:
    TimerCallback       m_callback;

    TimerQueueNode*     m_timerListNode;        // 定时器队列中的节点
    ULONG               m_nextTriggerMSecond;   // 下次触发的毫秒数
    ULONG               m_intervalMSecond;      // 触发间隔毫秒数
    bool                m_inTriggerList;

    std::list<TimerTrace> m_trace;            // 定时器跟踪

private:
    static bool         m_timerTraceOn;         // 开关
    static int          m_runingTimerCount;     // 运行中的timer数量
    static std::map<std::string, int>    m_runningTimerClassMap;  // 运行中的timer
};


