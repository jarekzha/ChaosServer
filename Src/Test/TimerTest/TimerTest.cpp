/*
 * TimerTest.cpp
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 
 */

#include <stdio.h>
#include <Time/Time.h>
#include <Time/Timer.h>
#include <Time/TimerManager.h>
#include <Util/TMTSingleton.h>
#include <Util/FakeRandom.h>


class A
{
public:
    A():count(0)
    {
        printf("A Start...\n");
        fflush(stdout);
        timer1.Start(TimerCallback(this, &A::Callback1), 2000);
        timer2 = new Timer;
    }

    ~A()
    {
        printf("A End...\n");
        fflush(stdout);
    }

    void                Start2()
    {
        timer2->Start(TimerCallback(this, &A::Callback2), 2000);
    }

    void                Stop2()
    {
        timer2->Stop();
    }

    void                Callback1()
    {
        printf("callback1 <time:%s>\n", Time::CurDateTimeStr());

        count++;
        if (count == 2)
        {
            timer1.ChangeIntervalMSecond(4000);
            Start2();
        }
        else if (count == 5)
        {
            Stop2();
        }
        else if (count == 10)
        {
            delete this;
            exit(0);
        }

    }

    void                Callback2()
    {
        printf("\e[33mcallback2 <time:%s>\e[m\n", Time::CurDateTimeStr());
        fflush(stdout);
    }

private:
    int                 count;
    Timer               timer1;
    Timer*              timer2;
};

class B
{
public:
    B(UINT interval):inter(interval)
    {
        endTime = Time::GetCurMSecond() + inter;
        printf("B obj:%lu, inter:%u curtime:%lu Start...\n", (ULONG)this, inter, Time::GetCurMSecond());

        fflush(stdout);
        timer1.Start(TimerCallback(this, &B::Callback1), inter);
    }

    ~B()
    {
        //printf("B End...\n");
        fflush(stdout);
    }

    void                Callback1()
    {
        int diff = (int)(endTime-Time::GetCurMSecond());
        printf("B obj:%lu, inter:%u curTime:%lu, endTime:%lu, diff:%d, intertick:%u difftick:%d End...\n",
                (ULONG)this, inter, Time::GetCurMSecond(), endTime, diff, inter/100, diff/100);
        printf("TICKTICK:%d\n", diff/100);

        delete this;
    }


private:
    Timer               timer1;

    UINT inter;
    ULONG endTime;
};



int main()
{
    TimerManager& timerMgr = TimerManager::Instance();
    timerMgr.Initialize(100, 3, 20);   // 100ms * 20^3

    printf("----All Start time:%lu\n",Time::GetCurMSecond());
    for (int i = 1; i < 8000; ++i)
    {
       // UINT inter = FakeRandom::Instance().Random(10000, 60000);
       // inter = inter - inter % 1000;
        new B(i * 100);
    }

    //new B(40100);
    Time::UpdateTime();
    printf("----All End time:%lu\n", Time::GetCurMSecond());

    while(true)
    {
        Time::UpdateTime();
        timerMgr.Run();

        Time::UsSleep(10);
    }

}


