/*
 * Thread.cpp
 *
 *  Created on: 2014年7月4日
 *      Author: kcinz
 */

#include "Thread.h"
#include "Util/Util.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function/function0.hpp>

#include "../Time/Time.h"

Thread::Thread()
{
    m_quitFlag = false;
    m_thread = NULL;
    m_needSleep = true;
}

Thread::~Thread()
{
    m_quitFlag = false;
    SAFE_DELETE(m_thread);
}

void Thread::Inner()
{
    while (!m_quitFlag)
    {
        m_needSleep = true;
        Routine();

        if(IsNeedSleep())
        {
            Time::MsSleep(1);
        }
        //boost::this_thread::sleep(boost::posix_time::milliseconds(0));
    }
}

void Thread::Start()
{
    boost::function0<void> f = boost::bind(&Thread::Inner, this);
    m_thread = new boost::thread(f);
}

/*
 * 此函数在主线程中调用，使得Inner函数可以完整执行一次
 * */
void Thread::Stop()
{
    m_quitFlag = true;
    OnStop();
    m_thread->join();
}

bool Thread::IsNeedSleep() const
{
    return m_needSleep;
}

void Thread::SetNeedSleep(bool needSleep)
{
    m_needSleep = needSleep;
}
