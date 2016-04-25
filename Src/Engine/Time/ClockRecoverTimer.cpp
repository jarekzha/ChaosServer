/*
 * ClockRecoverTimer.cpp
 *
 *  Created on: 2014年10月22日
 *      Author: liaojiayu
 */

#include "ClockRecoverTimer.h"

ClockConf::ClockConf()
{
    m_hour = -1;
    m_minute = -1;
    m_second = -1;
}

ClockConf::~ClockConf()
{
}

int ClockConf::GetHour() const
{
    return m_hour;
}

void ClockConf::SetHour(int hour)
{
    m_hour = hour;
}

int ClockConf::GetMinute() const
{
    return m_minute;
}

void ClockConf::SetMinute(int minute)
{
    m_minute = minute;
}

int ClockConf::GetSecond() const
{
    return m_second;
}

void ClockConf::SetSecond(int second)
{
    m_second = second;
}

//获取当天该时间点的毫秒数
ULONG ClockConf::GetTodayMSecond()
{
    time_t currSec = Time::GetCurSecond();
    struct tm temp;
    localtime_r(&currSec, &temp);

    temp.tm_hour = m_hour;
    temp.tm_min = m_minute;
    temp.tm_sec = m_second;

    time_t tNew = mktime(&temp);

    return tNew*1000;
}


ClockRecoverTimer::ClockRecoverTimer(RealFunction realFunction, RecoverAfterLoad afterLoad)
{
    m_realFunction = realFunction;
    m_recoverAfterLoad = afterLoad;
    m_interval = 0;
}

ClockRecoverTimer::~ClockRecoverTimer()
{
}

bool ClockRecoverTimer::InitClockList(ClockConfList& clockList)
{
    if(clockList.size() <= 0)
    {
        return false;
    }
    m_clockConfList = clockList;

    return true;
}
void ClockRecoverTimer::Start()
{
    ASSERT_RETURN_VOID(!m_clockConfList.empty());
    ULONG currMSec = Time::GetCurMSecond();
    ClockConfList::iterator it = m_clockConfList.begin();
    bool found = false;
    for(;it != m_clockConfList.end();++it)
    {
        ClockConf& clockConf = *it;
        if(currMSec < clockConf.GetTodayMSecond())
        {
            m_interval = clockConf.GetTodayMSecond() - currMSec;
            found = true;
            break;
        }
    }

    if(false == found)
    {
        ClockConf& clockConf = m_clockConfList.front();
        m_interval = clockConf.GetTodayMSecond()+24*3600*1000 - currMSec;
    }


    m_timer.Start(TimerCallback(this, &ClockRecoverTimer::RealTimerFunction),m_interval);
}

bool ClockRecoverTimer::Going()
{
    return m_timer.Going();
}

void ClockRecoverTimer::Stop()
{
    m_timer.Stop();
    m_interval = 0;
}

void ClockRecoverTimer::RealTimerFunction()
{
    m_realFunction();
    Stop();
    Start();
}

ULONG ClockRecoverTimer::GetNextTriggerSecond()
{
    return m_timer.GetNextTriggerMSecond() / 1000;
}

bool ClockRecoverTimer::BeforeSave()
{
    m_time = Time::GetCurSecond();
    return true;
}

int ClockRecoverTimer::_CalPassPointNum()
{
    ULONG currSec = Time::GetCurSecond();
    ULONG leftSec = Time::GetSecToNextDay(m_time);

    int remain = 0;
    //判断是否跨天
    if((currSec - m_time) > leftSec)
    {
        //跨天
        struct tm lastTime;
        Time::GetTm( m_time, &lastTime);

        int pass = 0;

        //计算上次下线时距离当天结束还有几个时间点
        ClockConfList::iterator it1 = m_clockConfList.begin();
        for(;it1 != m_clockConfList.end();++it1)
        {
            ClockConf& clockConf = *it1;
            if(lastTime.tm_hour == clockConf.GetHour())
            {
                if(lastTime.tm_min == clockConf.GetMinute())
                {
                    if(lastTime.tm_sec <= clockConf.GetSecond())
                    {
                        break;
                    }
                }
                else if(lastTime.tm_min < clockConf.GetMinute())
                {
                    break;
                }
            }
            else if(lastTime.tm_hour < clockConf.GetHour())
            {
                break;
            }

            pass++;
        }

        remain += m_clockConfList.size() - pass;

        //计算上线后当天经过的时间点
        pass = 0;
        ULONG currMSec = Time::GetCurMSecond();
        ClockConfList::iterator it2 = m_clockConfList.begin();
        for(;it2 != m_clockConfList.end();++it2)
        {
            ClockConf& clockConf = *it2;
            if(currMSec < clockConf.GetTodayMSecond())
            {
                break;
            }

            pass++;
        }

        remain += pass;

        //计算上次下线到本次上次所经历的整天数
        int leftSec = Time::GetSecToNextDay(m_time);
        int passSec = 24*3600 - Time::GetSecToNextDay(currSec);
        int totalDay = (currSec - m_time - leftSec - passSec)/(24*3600);
        remain += totalDay*m_clockConfList.size();
    }
    else
    {
        struct tm lastTime;
        Time::GetTm( m_time, &lastTime);
        int pass = 0;
        ClockConfList::iterator it1 = m_clockConfList.begin();
        for(;it1 != m_clockConfList.end();++it1)
        {
            ClockConf& clockConf = *it1;
            bool flag = false;
            if(lastTime.tm_hour == clockConf.GetHour())
            {
                if(lastTime.tm_min == clockConf.GetMinute())
                {
                    if(lastTime.tm_sec <= clockConf.GetSecond())
                    {
                        flag = true;
                    }
                }
                else if(lastTime.tm_min < clockConf.GetMinute())
                {
                    flag = true;
                }
            }
            else if(lastTime.tm_hour < clockConf.GetHour())
            {
                flag = true;
            }

            if(true == flag)
            {
                if(Time::GetCurMSecond() >= clockConf.GetTodayMSecond())
                {
                    pass++;
                }
                else
                {
                    break;
                }
            }
        }
        remain = pass;
    }
    return remain;
}
bool ClockRecoverTimer::LoadPostPorcess()
{
    if(0 == m_interval)
    {
        return true;
    }
    m_recoverAfterLoad(_CalPassPointNum());

    Start();
    return true;
}
