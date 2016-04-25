/*
 * RequestManager.cpp
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Util/Util.h>
#include <Engine/Log/LogMacro.h>
#include <Request/Request.h>
#include <Request/RequestManager.h>
#include <Engine/Thread/ThreadMutex.h>
#include <vector>

using namespace std;


RequestManager::RequestManager()
{
    maxRequestID = 0;
}

RequestManager::~RequestManager()
{
    ReleaseAllRequest();

    while (!freeRequestQueue.empty())
    {
        Request* requset = freeRequestQueue.front();
        Delete(requset);

        freeRequestQueue.pop();
    }
}

bool RequestManager::Initialize(int requestCount)
{
    for (int i = 0; i < requestCount; ++i)
    {
        Request* request = New<Request>();
        ASSERT_RETURN(request != NULL, false);

        if (!request->Initialize())
        {
            return false;
        }

        freeRequestQueue.push(request);
    }

    return true;
}

Request* RequestManager::CreateRequest()
{
    if (freeRequestQueue.empty())
    {
        return NULL;
    }

    ThreadMutexGuard lock(threadMutex);

    int requestID = AllocateValidRequestID();
    ASSERT_RETURN(requestID != 0, NULL);

    // 从空闲队列中取出
    Request* request = freeRequestQueue.front();
    freeRequestQueue.pop();

    request->SetRequestID(requestID);

    // 插入已用map中
    usedRequestMap.insert(RequestIDRequestMap::value_type(requestID, request));

    LOG_TRACE("Request<%s> create, cur used<count:%d> free<count:%d>",
            request->BriefInfo().c_str(), GetUsedRequestCount(), GetFreeRequestCount());

    return request;
}

void RequestManager::ReleaseRequest(int requestID)
{
    ASSERT_RETURN_VOID(requestID != 0);

    ThreadMutexGuard lock(threadMutex);

    RequestIDRequestMap::iterator iter = usedRequestMap.find(requestID);
    if (iter == usedRequestMap.end())
    {
        LOG_ERR("Release request<requestID:%d> fail, no request found", requestID);
        assert(false);
        return;
    }

    // 从已用map中删除
    Request* request = iter->second;
    usedRequestMap.erase(iter);

    // 插入空闲列表
    freeRequestQueue.push(request);

    LOG_TRACE("Request<%s> release, cur used<count:%d> free<count:%d>",
            request->BriefInfo().c_str(), GetUsedRequestCount(), GetFreeRequestCount());

    request->Reset();
}

Request* RequestManager::GetRequest(int requestID)
{
    ASSERT_RETURN(requestID != 0, NULL);

    ThreadMutexGuard lock(threadMutex);

    RequestIDRequestMap::iterator iter = usedRequestMap.find(requestID);
    if (iter == usedRequestMap.end())
    {
        return NULL;
    }

    return iter->second;
}

int RequestManager::GetFreeRequestCount()
{
    return freeRequestQueue.size();
}

int RequestManager::GetUsedRequestCount()
{
    return usedRequestMap.size();
}

int RequestManager::AllocateRequestID()
{
    int requestID = ++maxRequestID;
    if (0 == requestID)
    {
        requestID = ++maxRequestID;
    }

    return requestID;
}

int RequestManager::AllocateValidRequestID()
{
    int tryNum = usedRequestMap.size() + 1;
    bool foundValidID = false;
    int validRequestID = 0;
    for (int i = 0; i < tryNum; ++i)
    {
        validRequestID = AllocateRequestID();
        if (GetRequest(validRequestID) == NULL)
        {
            foundValidID = true;
            break;
        }
    }

    ASSERT_RETURN(foundValidID, 0);
    return validRequestID;
}

void RequestManager::ReleaseAllRequest()
{
    vector<int> usedRequestID;
    for (RequestIDRequestMap::iterator iter = usedRequestMap.begin();
            iter != usedRequestMap.end(); ++iter)
    {
        usedRequestID.push_back(iter->second->GetRequestID());
    }

    for (size_t i = 0; i < usedRequestID.size(); ++i)
    {
        ReleaseRequest(usedRequestID[i]);
    }
}




