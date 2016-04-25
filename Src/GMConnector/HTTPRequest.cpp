/*
 * HTTPRequest.cpp
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#include "HTTPRequest.h"
#include <string>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <Engine/IPC/IPCHandle.h>

#include <Engine/Object/New.h>
#include <Engine/Util/HTTPUtil.h>
#include <Engine/Log/LogMacro.h>
#include "GMConnectorConfig.h"
#include "GMConnector.h"

#define MAX_HTTPREQUEST_LEN (1024)

HTTPRequest::HTTPRequest()
    :httpRequstBuf(NULL), httpRequestBuflen(0)
{
    httpRequstBuf = NewArray<char>(MAX_HTTPREQUEST_LEN);
}

HTTPRequest::~HTTPRequest()
{
    DeleteArray(httpRequstBuf);

    FCGX_Free(&fastcgiRequestHandle, 1);
}

bool HTTPRequest::Initialize()
{
    if (FCGX_InitRequest(&fastcgiRequestHandle, 0, 0) != 0)
    {
        LOG_ERR("Request initialize fail:%s", strerror(errno));
        return false;
    }

    return true;
}

bool HTTPRequest::DispatchHTTPRequest()
{
    int ret = FCGX_Accept_r(&fastcgiRequestHandle);
    if (ret < 0)
    {
        LOG_WARN("Request wait fail<result:%d>", ret);
        return false;
    }

    if (!ReadRequest())
    {
        Finish();
        return false;
    }

    //handle http request
    HandleHTTPRequest();

    //response
    DefaultResponse();

    return true;
}

void HTTPRequest::Finish()
{
    FCGX_Finish_r(&fastcgiRequestHandle);
}

bool HTTPRequest::ReadRequest()
{
    memset(httpRequstBuf, 0, MAX_HTTPREQUEST_LEN);

    // 读取全包
    httpRequestBuflen = FCGX_GetStr(httpRequstBuf, MAX_HTTPREQUEST_LEN, fastcgiRequestHandle.in);
    if (MAX_HTTPREQUEST_LEN == httpRequestBuflen)
    {
        LOG_WARN("[FAST-CGI] read request fail, buffer is small");
        return false;
    }

    return true;
}

bool HTTPRequest::HandleHTTPRequest()
{
    //TO-DO
    if (0 == httpRequestBuflen)
    {
        LOG_TRACE("Browser already closed");
        return false;
    }

    //pre-procedure
    std::vector<std::string> httpUrlVec;
    std::string httpUrl(httpRequstBuf, httpRequestBuflen);
    if (!HTTPUtil::Instance().Split(httpUrl, "&", httpUrlVec))
    {
        LOG_WARN("[HTTP-HANDLE] pre-procedure fail, httpUrl[%s]", httpUrl.c_str());
        return false;
    }

    std::string commandStr = httpUrlVec[0];
    httpUrlVec.clear();
    if (!HTTPUtil::Instance().Split(commandStr, "=", httpUrlVec))
    {
        LOG_WARN("[HTTP-HANDLE] parse command str[%s] fail", commandStr.c_str());
        return false;
    }

    if (httpUrlVec.size() < 2)
    {
        LOG_WARN("[HTTP-HANDLE] parse command str[%s] fail", commandStr.c_str());
        return false;
    }

    std::string& commandValStr = httpUrlVec[1];

    return ForwardToGameSvr(commandValStr);
}

bool HTTPRequest::ForwardToGameSvr(const std::string& commandStr)
{
    if (commandStr.empty() || commandStr.length() > (MAX_HTTPREQUEST_LEN + sizeof(UINT)))
    {
        return false;
    }

    LOG_TRACE("[FOWARD-HTTP-REQUEST] command-val[%s] len[%u] forward to game", commandStr.c_str(), (uint)commandStr.size());

    const GMConnectorConfigItem& config = theGMConnector.GetConfigItem();
    IPCHandle& ipcHandle = theGMConnector.GetIPCHandle();

    return ipcHandle.SendData(config.GameServerProcID, commandStr.c_str(), commandStr.size() + sizeof(UINT));
}

bool HTTPRequest::DefaultResponse()
{
    FCGX_PutS("Content-type:text/html\r\n\r\n", fastcgiRequestHandle.out);

    char resUrl[MAX_HTTPREQUEST_LEN] = {0};
    const GMConnectorConfigItem& config = theGMConnector.GetConfigItem();
    snprintf(resUrl, sizeof(resUrl), "<script>window.setTimeout(\"location.href = '%s'\", 500);</script>\n", config.HTTPUrl.c_str());
    FCGX_PutS(resUrl, fastcgiRequestHandle.out);

    return true;
}
