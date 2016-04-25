/*
 * AsyncTask.cpp
 *
 *  Created on: 2014年7月5日
 *      Author: kcinz
 *      Modified by: Jarek     2015.11.5
 */

#include <AsyncData/AsyncTask.h>
#include <utility>


AsyncTask::AsyncTask()
{
    asyncDataMap.clear();
}

AsyncTask::~AsyncTask()
{
    UnregisterAllAsyncData();
}

void AsyncTask::HandleAsyncDataEvent(AsyncDataEvent event)
{
    switch (event)
    {
        case AsyncDataEvent_GetTimeout:
            OnGetDataTimeOut();
            OnRelease();
            break;

        case AsyncDataEvent_GetComplete:
            OnAsyncDataComplete();
            break;

        default:
            break;
    }
}

void AsyncTask::StartExecuteAsyncTask()
{
    AsyncDataList asyncDataList;
    OnGetCreateAsyncData(asyncDataList);

    for (AsyncDataList::iterator iter = asyncDataList.begin(); iter != asyncDataList.end(); ++iter)
    {
        AsyncData* asyncData = *iter;
        RegisterAsyncData(*asyncData);           // Data和Task互相监听
    }

    bool needWaitAsyncData = false;
    for (AsyncDataList::iterator iter = asyncDataList.begin(); iter != asyncDataList.end(); ++iter)
    {
        AsyncData* asyncData = *iter;

        switch (asyncData->GetDataState())
        {
            case AsyncDataState_Empty:
            {
                asyncData->StartLoad();     // 开始异步，大部分时候是发消息
                needWaitAsyncData = true;
                continue;
            }
            case AsyncDataState_Getting:
            {
                needWaitAsyncData = true;   // 需要异步获取
                continue;
            }
            case AsyncDataState_Complete:
            {
                continue;
            }
        }
    }

    if (!needWaitAsyncData)
    {
        OnExecute();
        OnRelease();
    }
}

void AsyncTask::RegisterAsyncData(AsyncData& asyncData)
{
    asyncDataMap.insert(AsyncDataMap::value_type(asyncData.GetObjectID(), &asyncData));
    asyncData.RegisterAsyncTask(*this);
}

void AsyncTask::UnregisterAllAsyncData()
{
    for (AsyncDataMap::iterator iter = asyncDataMap.begin(); iter != asyncDataMap.end(); ++iter)
    {
        AsyncData* data = iter->second;
        data->UnregisterAsyncTask(*this);
    }

    asyncDataMap.clear();
}

void AsyncTask::OnAsyncDataComplete()
{
    for (AsyncDataMap::iterator iter = asyncDataMap.begin(); iter != asyncDataMap.end(); ++iter)
    {
        AsyncData* asyncData = iter->second;
        if (asyncData->GetDataState() != AsyncDataState_Complete)
        {
            return;
        }
    }

    OnExecute();
    OnRelease();
}
