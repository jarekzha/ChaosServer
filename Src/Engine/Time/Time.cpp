#include <Common.h>
#include <Time/Time.h>

time_t Time::currentTime = time(NULL);
struct timeval Time::currentTimeVal = Time::InitialTimeVal();
int Time::utcOff = Time::InitialZoneOff();

char* Time::CurDateTimeStr(void)
{
    return DateTimeStr(&currentTime);
}

char* Time::DateTimeStr(time_t* inTime)
{
    static char s[50];
    struct tm curr;
    
    curr = *localtime(inTime);

    if (curr.tm_year > 50)
    {
        snprintf(s, sizeof(s)-1,
                    "%04d-%02d-%02d %02d:%02d:%02d", 
                    curr.tm_year+1900, curr.tm_mon+1, curr.tm_mday,
                    curr.tm_hour, curr.tm_min, curr.tm_sec);
    }
    else
    {
        snprintf(s, sizeof(s)-1,
                    "%04d-%02d-%02d %02d:%02d:%02d", 
                    curr.tm_year+2000, curr.tm_mon+1, curr.tm_mday,
                    curr.tm_hour, curr.tm_min, curr.tm_sec);
    }
                
    return s;
}

int Time::MsSleep(unsigned long sleepTime)
{
    struct timeval vtime;

    vtime.tv_sec = sleepTime / 1000;
    vtime.tv_usec = (sleepTime % 1000) * 1000;
    select(0, NULL, NULL, NULL, &vtime);
    return 0;
}

unsigned int Time::MsPass(struct timeval* time1, struct timeval* time2)
{
    int secPass;
    int msecPass;

    secPass = (int)time1->tv_sec - (int)time2->tv_sec;
    if (secPass < 0)
        return  MsPass(time2, time1);

    msecPass = ((int)time1->tv_usec - (int)time2->tv_usec) / 1000;

    if (secPass > 0)
        return  secPass * 1000 + msecPass;
    else    // secPass == 0
        return abs(msecPass);
}

int Time::CompareTimeVal(struct timeval* time1, struct timeval* time2)
{
    if (time1 == time2) return 0;
    if (time1->tv_sec > time2->tv_sec) return 1;
    if (time1->tv_sec < time2->tv_sec) return -1;

    if (time1->tv_sec == time2->tv_sec)
    {
        if (time1->tv_usec > time2->tv_usec)
            return 1;
        else
            return -1;
    }

    return 0;
}

void Time::UpdateTime()
{
    currentTime = time(NULL);
    gettimeofday(&currentTimeVal, NULL);
}

struct timeval& Time::InitialTimeVal()
{
    static struct timeval tmp;
    gettimeofday(&tmp, NULL);
    return tmp;
}

int Time::InitialZoneOff()
{
    time_t      lNow;
    struct tm   curr;

    time(&lNow);
    curr = *localtime(&lNow);

    return curr.tm_gmtoff;
}

int Time::UsSleep(unsigned long us)
{
    struct timeval vtime;

    vtime.tv_sec = 0;
    vtime.tv_usec = us;
    select(0, NULL, NULL, NULL, &vtime);
    return 0;
}

bool Time::IsSameDay(time_t time1, time_t time2)
{
    struct tm stTm1;
    struct tm stTm2;

    localtime_r(&time1, &stTm1);
    localtime_r(&time2, &stTm2);

    if(stTm1.tm_year == stTm2.tm_year &&
       stTm1.tm_yday == stTm2.tm_yday)
    {
        return true;
    }

    return false;
}

int Time::DayDiff(time_t dest, time_t src)
{
    ULONG timeDay = 24*60*60;
    return (src + Time::utcOff) / timeDay - (dest + Time::utcOff) / timeDay;
}

int Time::GetSecToNextDay(time_t time1)
{
    struct tm next;

    localtime_r(&time1, &next);


    next.tm_hour = 0;
    next.tm_min = 0;
    next.tm_sec = 0;

    time_t tNew = mktime(&next);
    tNew += 24*3600;

    return tNew-time1;
}

void Time::GetTm( time_t time1, struct tm * ptrTM)
{
    if( NULL == ptrTM)
    {
        return;
    }

    localtime_r(&time1, ptrTM);
}

ULONG Time::GetCurSecond()
{
    return currentTimeVal.tv_sec;
}

ULONG Time::GetCurMSecond()
{
    return currentTimeVal.tv_sec * 1000 + currentTimeVal.tv_usec / 1000;
}

ULONG Time::GetLocalCurSecond()
{
    return currentTimeVal.tv_sec + utcOff;
}

ULONG Time::GetLocalCurMSecond()
{
    return (currentTimeVal.tv_sec + utcOff) * 1000 + currentTimeVal.tv_usec / 1000;
}

bool Time::StringToTime(const std::string& dateString, time_t& outTime)
{
    char* beginPos = (char*)dateString.c_str();
    char* pos = strstr(beginPos, "-");
    if (pos == NULL)
    {
        return false;
    }

    int year = atoi(beginPos);
    int month = atoi(pos + 1);
    pos = strstr(pos + 1,"-");
    if (pos == NULL)
    {
        return false;
    }
    int day = atoi(pos + 1);
    int hour = 0;
    int min = 0;
    int sec = 0;
    pos = strstr(pos + 1," ");
    //为了兼容有些没精确到时分秒的
    if (pos != NULL)
    {
        hour = atoi(pos + 1);
        pos = strstr(pos + 1,":");
        if (pos != NULL)
        {
            min = atoi(pos + 1);
            pos = strstr(pos + 1,":");
            if (pos != NULL)
            {
                sec = atoi(pos + 1);
            }
        }
    }

    struct tm sourcedate;
    bzero((void*)&sourcedate, sizeof(sourcedate));
    sourcedate.tm_sec = sec;
    sourcedate.tm_min = min;
    sourcedate.tm_hour = hour;
    sourcedate.tm_mday = day;
    sourcedate.tm_mon = month - 1;
    sourcedate.tm_year = year - 1900;
    outTime = mktime(&sourcedate);

    return true;
}







