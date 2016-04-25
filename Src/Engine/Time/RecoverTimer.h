/*
 * RecoverTimer.h
 *
 *  Created on: 2014年9月26日
 *      Author: kcinz
 */

#ifndef RECOVERTIMER_H_
#define RECOVERTIMER_H_

#include <Time/Timer.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>

typedef boost::function<void()> RealFunction;
typedef boost::function<void(int)> RecoverAfterLoad;

//#define SHOW_DEBUG_LOG

class RecoverTimer
{
public:
    RecoverTimer(RealFunction realFunction, RecoverAfterLoad afterLoad);
    virtual ~RecoverTimer();

private:
    RecoverTimer();

public:
    void Start(ULONG intervalMSecond);
    void Stop();

public:
    void RealTimerFunction();       // 正式的时间处理函数
    void RemainTimerFunction();     // 恢复时的剩余周期时间处理函数
    bool LoadPostPorcess();                 // 序列化load的后处理函数

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::make_nvp("LastTimeMS",m_lastTimeMs);
        ar & boost::serialization::make_nvp("RealIntervalMS",m_realIntervalMs);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::make_nvp("LastTimeMS",m_lastTimeMs);
        ar & boost::serialization::make_nvp("RealIntervalMS",m_realIntervalMs);

        if (false == LoadPostPorcess())
            assert(false);      // 需要修改为抛异常
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER();

    ULONG GetNextTimePoint();

    void SetIsLoadResume(bool isResume);

protected:
    Timer m_timer;              // 计时器

    ULONG m_loadTimeMs;          // load的时间，load时获取
    ULONG m_lastTimeMs;          // 最后一次触发的时间，内部记录，并序列化
    ULONG m_realIntervalMs;      // 真实的间隔时间，外部传入，并序列化

    RealFunction m_realFunction;            // Timer执行的真正函数
    RecoverAfterLoad m_recoverAfterLoad;    // 由load的时间开始做恢复的函数

    bool m_isLoadResume;        // 在运行过RecoverAfterLoad之后是否还需要继续
};

#endif /* RECOVERTIMER_H_ */
