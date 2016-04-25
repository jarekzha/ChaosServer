/*
 * AsyncData.cpp
 *
 *  Created on: 2014年7月4日
 *      Author: kcinz
 *      Modified by: Jarek     2015.11.5
 */

#include <AsyncData/AsyncData.h>
#include <AsyncData/AsyncTask.h>
#include <Time/TimerCallback.h>
#include <utility>
#include <Util/Memcacheable.h>

AsyncData::AsyncData()
{
    state               = AsyncDataState_Empty;
    expireIntervalMS    = 0;
    getDataIntervalMS   = GET_ASYNC_DATA_TIMEOUT_MS;
}

AsyncData::~AsyncData()
{
    // NULL
}

int AsyncData::GetExpiresIntervalMS() const
{
    return expireIntervalMS;
}

void AsyncData::SetExpiresIntervalMS(int expiresInterval)
{
    expireIntervalMS = expiresInterval;
}

int AsyncData::GetGetDataIntervalMS() const
{
    return getDataIntervalMS;
}

void AsyncData::StartLoad()
{
    getDataTimeout.Start(TimerCallback(this, &AsyncData::OnTimerGetDataTimeout), getDataIntervalMS);
    state = AsyncDataState_Getting;

    OnStartLoad();
}

void AsyncData::EndLoad()
{
    state = AsyncDataState_Complete;
    getDataTimeout.Stop();

    // 开始过期计时器
    if (0 == expireIntervalMS)
    {
        expireTimer.Start(TimerCallback(this, &AsyncData::OnTimerExpire), expireIntervalMS);
    }

    // 传递事件到AsyncTask
    DispatchAsyncDataEvent(AsyncDataEvent_GetComplete);
}

void AsyncData::SetGetDataIntervalMS(int getDataInterval)
{
    getDataIntervalMS = getDataInterval;
}

AsyncDataState AsyncData::GetDataState() const
{
    return state;
}

void AsyncData::RegisterAsyncTask(AsyncTask& asyncTask)
{
    AsyncTaskMap::iterator iter = asyncTaskMap.find(asyncTask.GetObjectID());
    if (iter != asyncTaskMap.end())
    {
        return;
    }

    asyncTaskMap.insert(AsyncTaskMap::value_type(asyncTask.GetObjectID(), &asyncTask));
}

void AsyncData::UnregisterAsyncTask(AsyncTask& asyncTask)
{
    asyncTaskMap.erase(asyncTask.GetObjectID());
}

void AsyncData::DispatchAsyncDataEvent(AsyncDataEvent asyncDataEvent)
{
    AsyncTaskMap::iterator iter = asyncTaskMap.begin();
    for (; iter != asyncTaskMap.end(); ++iter)
    {
        AsyncTask* asyncTask = iter->second;
        asyncTask->HandleAsyncDataEvent(asyncDataEvent);
    }
}

void AsyncData::OnTimerGetDataTimeout()
{
    DispatchAsyncDataEvent(AsyncDataEvent_GetTimeout);

    // 如果没有异步任务并且有超时，就回收
    if (asyncTaskMap.empty())
    {
        Release();
    }
}

void AsyncData::OnTimerExpire()
{
    // 永不过期
    if (0 == expireIntervalMS)
    {
        return;
    }

    // 如果没有异步任务并且有超时，就回收
    if (asyncTaskMap.empty())
    {
        expireTimer.Stop();
        Release();
    }
}




