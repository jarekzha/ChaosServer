/*
 * DelayReleaseRequest.cpp
 *
 *  Created on: 2014年8月5日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Request/DelayReleaseRequest.h>
#include <Request/Request.h>
#include <Request/RequestManager.h>

DelayReleaseRequest::DelayReleaseRequest(Request* req)
    : requestID(0)
{
    ASSERT_RETURN_VOID(req != NULL);

    requestID = req->GetRequestID();
}

void DelayReleaseRequest::Invoke()
{
    RequestManager::Instance().ReleaseRequest(requestID);
}




