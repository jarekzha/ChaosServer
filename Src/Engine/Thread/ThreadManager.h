/*
 * ThreadManager.h
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Thread/Thread.h>
#include <Util/TMTSingleton.h>
#include <list>

class ThreadManager : public TMTSingleton<ThreadManager>
{
public:
                        ThreadManager();
    virtual             ~ThreadManager();

    void                AddThread(Thread& thread);

    void                Start();

    void                Stop();

private:
    typedef std::list<Thread*> ThreadList;
    ThreadList          threadList;
};



