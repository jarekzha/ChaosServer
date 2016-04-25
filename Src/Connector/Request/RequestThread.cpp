/*
 * RequestThread.cpp
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Time/Time.h>
#include <Framework/Connector.h>
#include <Request/Request.h>
#include <Request/RequestManager.h>
#include <Request/RequestThread.h>

RequestThread::RequestThread()
{
    // NULL
}

RequestThread::~RequestThread()
{
    // NULL
}

void RequestThread::Routine()
{
    static RequestManager& requestManager = theConnector.GetRequestManager();

    Request* request = requestManager.CreateRequest();

    // 无可用request
    if (NULL == request)
    {
        Time::MsSleep(10);
        return;
    }

    // 等待请求
    if (!request->WaitAccept())
    {
        requestManager.ReleaseRequest(request->GetRequestID());
        return;
    }

    request->SendToGameServer();
}





