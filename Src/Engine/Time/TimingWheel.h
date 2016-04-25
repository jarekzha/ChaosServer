/*
 * TimingWheel.h
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 单个时间轮
 */
#pragma once

#include <Time/Timer.h>
#include <Time/TimingBucket.h>
#include <Util/Util.h>
#include <list>
#include <vector>

class Timer;

/*
 * 单个时间轮
 */
class TimingWheel
{
public:
                        TimingWheel();
    virtual             ~TimingWheel();

    bool                Initialize(UINT index, UINT bucketNum);

    // 插入定时器
    void                InsertTimer(Timer* timer);

    // 取出当前指向的bucket中的所有定时器，取出时清除挂接关系
    void                TakeOutCursorBucketAllTimer(std::list<Timer*>& timerQueue);

    // 走一个tick，轮子不同，tick单位不同，return是否需要进位
    bool                DoOneTick();

    UINT                GetMaxTickNum();

    UINT                GetBucketUnitOfTickNum();

private:
    typedef std::vector<TimingBucket> TimingBucketVector;
    TimingBucketVector  m_bucket;
    UINT                m_cursor;           // 本轮子光标
    UINT                m_index;            // 本轮子在时间轮中的序号
    UINT                m_maxTickNum;       // 本轮子能够表示的最大tick数量
    UINT                m_minTickNum;       // 本轮子能够表示的最小tick数量
    UINT                m_bucketUnitOfTickNum;  // 本轮子中单个bucket的单位，其所表示的tick数量
};



