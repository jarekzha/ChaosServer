/*
 * ThreadManager.cpp
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 
 */

#include <Thread/ThreadManager.h>
#include <Util/Util.h>
#include <Object/New.h>

ThreadManager::ThreadManager()
{
    // NULL
}

ThreadManager::~ThreadManager()
{
    for (ThreadList::iterator iter = threadList.begin(); iter != threadList.end(); ++iter)
    {
        Delete(*iter);
    }
}

void ThreadManager::AddThread(Thread& thread)
{
    threadList.push_back(&thread);
}

void ThreadManager::Start()
{
    for (ThreadList::iterator iter = threadList.begin(); iter != threadList.end(); ++iter)
    {
        Thread* thread = *iter;
        thread->Start();
    }
}

void ThreadManager::Stop()
{
    for (ThreadList::iterator iter = threadList.begin(); iter != threadList.end(); ++iter)
    {
        Thread* thread = *iter;
        thread->Stop();
    }
}

