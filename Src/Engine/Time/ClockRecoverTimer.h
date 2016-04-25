/*
 * ClockRecoverTimer.h
 *
 *  Created on: 2014年10月22日
 *      Author: liaojiayu
 */

#ifndef CLOCKRECOVERTIMER_H_
#define CLOCKRECOVERTIMER_H_
#include <list>

#include "RecoverTimer.h"
#include "Time.h"

using namespace std;

class ClockConf
{
public:
    ClockConf();
    ~ClockConf();
    int GetHour() const;
    void SetHour(int hour);
    int GetMinute() const;
    void SetMinute(int minute);
    int GetSecond() const;
    void SetSecond(int second);

    //获取当天该时间点的毫秒数
    ULONG GetTodayMSecond();

private:
    int m_hour;    //触发点数 24小时制
    int m_minute;  //触发分钟
    int m_second;  //触发秒杀
};

typedef list<ClockConf> ClockConfList;
//每日定点触发timer
class ClockRecoverTimer
{
public:
    ClockRecoverTimer(RealFunction realFunction, RecoverAfterLoad afterLoad);
    virtual ~ClockRecoverTimer();


public:
    bool InitClockList(ClockConfList& clockList);           //初始触发的时间序列
    void Start();
    void Stop();

    ULONG GetNextTriggerSecond();           // 获取下次触发时的秒数
public:
    void RealTimerFunction();
    bool LoadPostPorcess();                 // 序列化load的后处理函数

    bool BeforeSave();

    bool Going();

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        const_cast<ClockRecoverTimer*>(this)->BeforeSave();
        ar & boost::serialization::make_nvp("Time",m_time);
        ar & boost::serialization::make_nvp("IntervalTime",m_interval);
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::make_nvp("Time",m_time);
        ar & boost::serialization::make_nvp("IntervalTime",m_interval);

        if (false == LoadPostPorcess())
            assert(false);      // 需要修改为抛异常
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
    int _CalPassPointNum();  //计算反序列化后经历过的时间点数
private:
    //RecoverTimer m_recoverTimer;
    Timer    m_timer;
    ClockConfList m_clockConfList;
    ULONG    m_time;   //数据销毁时的时间 S
    ULONG    m_interval;    //触发间隔

    RealFunction m_realFunction;            // Timer执行的真正函数
    RecoverAfterLoad m_recoverAfterLoad;    // 由load的时间开始做恢复的函数
};

#endif /* CLOCKRECOVERTIMER_H_ */
