/*
 * Thread.h
 *
 *  Created on: 2014年7月4日
 *      Author: kcinz
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <boost/thread/thread.hpp>

class Thread
{
public:
    Thread();
    virtual ~Thread();

    virtual void Routine() = 0;

    virtual void OnStop() = 0;

    void Inner();

    void Start();

    void Stop();

    bool IsNeedSleep() const;

    void SetNeedSleep(bool needSleep);

private:
    bool m_quitFlag;

    boost::thread* m_thread;

    bool m_needSleep;
};

#endif /* THREAD_H_ */
