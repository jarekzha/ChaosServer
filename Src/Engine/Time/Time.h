#pragma once

#include <time.h>
#include <sys/time.h>
#include <Util/Util.h>
#include <string>

class Time
{
public:
    /*! 
    @brief 把当前时间转换成字符串输出
    @return 时间字符串
    */
    static char*        CurDateTimeStr(void);

    /*! 
    @brief 把输入时间变换成字符串形式输出
    @param[in]  mytime:输入时间   
    @return 
    */
    static char*        DateTimeStr(time_t *inTime);

    /*! 
    @brief 休眠ms
    @param[in]  ulMs:需要休眠的ms   
    @return 0
    */
    static int          MsSleep(unsigned long sleepTime);

    /*!
     @brief 休眠纳秒
     @param[in]  ulMs:需要休眠的纳秒
     @return 0
     */
    static int          UsSleep(unsigned long us);

    /*! 
    @brief 计算两个时间点之间相差的MS数
    @param[in]  pstTv1  第一个时间点 
    @param[in]  pstTv2  第二个时间点 
    @return 相差的MS数
    */
    static unsigned int MsPass(struct timeval* time1, struct timeval* time2);

    static int          CompareTimeVal(struct timeval* time1, struct timeval* time2);

    static void         UpdateTime();

    static struct timeval& InitialTimeVal();

    static int          InitialZoneOff();
	
    /*! 
    @brief 判断两个时间点是否跨天
    @param[in]  两个时间点   
    @return 是否跨天
    */
    static bool         IsSameDay(time_t time1, time_t time2);

    //计算src到dest之间相差的天数
    static int          DayDiff(time_t dest, time_t src);
	/*! 
    @brief 计算到第二天0点的秒数
    @param[in]  两个时间点   
    @return 距离第二天的秒数
    */
    static int          GetSecToNextDay(time_t time1);

	/*! 
    @brief 时间结构转换
    @param[in]  
    @return 
    */
    static void         GetTm( time_t time1, struct tm * ptrTM);

    static ULONG        GetCurSecond();
    static ULONG        GetCurMSecond();

    static ULONG        GetLocalCurSecond();
    static ULONG        GetLocalCurMSecond();

    static bool         StringToTime(const std::string& dateString, time_t& outTime);

private:
    static int              utcOff;         // 和UTC时区之前的差值
    static time_t           currentTime;
    static struct timeval   currentTimeVal;
};

