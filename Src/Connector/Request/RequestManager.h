/*
 * RequestManager.h
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/ThreadMutex.h>
#include <Engine/Util/TMTSingleton.h>
#include <Request/Request.h>
#include <map>
#include <queue>

class Request;


class RequestManager : public TMTSingleton<RequestManager>
{
public:
    RequestManager();
    virtual             ~RequestManager();

public:
    bool                Initialize(int requestCount);

    Request*            CreateRequest();

    void                ReleaseRequest(int requestID);

    Request*            GetRequest(int requestID);

    int                 GetFreeRequestCount();

    int                 GetUsedRequestCount();

private:
    int                 AllocateRequestID();

    int                 AllocateValidRequestID();

    void                ReleaseAllRequest();

private:
    typedef std::queue<Request*> RequestQueue;
    typedef std::map<int, Request*> RequestIDRequestMap;    // requestID->request

    RequestQueue        freeRequestQueue;
    RequestIDRequestMap usedRequestMap;
    int                 maxRequestID;

    ThreadMutex         threadMutex;
};
