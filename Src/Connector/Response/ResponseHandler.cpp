/*
 * ResponseHandler.cpp
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/IPC/IPCHandle.h>
#include <Engine/Log/LogMacro.h>
#include <Framework/Connector.h>
#include <Request/Request.h>
#include <Request/RequestManager.h>
#include <Response/ResponseHandler.h>
#include <cstdio>
#include <string>

ResponseHandler::ResponseHandler()
{
    // NULL
}

ResponseHandler::~ResponseHandler()
{
    // NULL
}

void ResponseHandler::HandleResponse()
{
    if (!RecvFromGameServer())
    {
        return;
    }

    DoResponse();
}

bool ResponseHandler::RecvFromGameServer()
{
    static IPCHandle& ipcHandle = theConnector.GetIPCHandle();

    IPCHandleRes ret = ipcHandle.GetCompleteMessage(responseMsgSet);
    if (IPCHandleRes_Error == ret)
    {
        LOG_ERR("IPCHandle error fail");
        return false;
    }

    return IPCHandleRes_Success == ret;
}

void ResponseHandler::DoResponse()
{
    static RequestManager& requestMgr = theConnector.GetRequestManager();

    int requestID = responseMsgSet.extra().cs_msg().request_id();
    Request* request = requestMgr.GetRequest(requestID);
    if (NULL == request)
    {
        LOG_ERR("Response fail, used request<ID:%d> not found", requestID);
        LOG_TRACE("Response fail, msgSet<%s>", responseMsgSet.DebugString().c_str());
        return;
    }

    request->DoResponse(responseMsgSet);
    requestMgr.ReleaseRequest(request->GetRequestID());
}

