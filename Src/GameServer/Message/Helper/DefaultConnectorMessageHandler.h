/*
 * DefaultConnectorMessageHelper.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/AsyncData/AsyncTask.h>
#include <Engine/Message/Handler/MessageHandler.h>

class CSRequest;
class PlayerSession;

/*
 * 默认CS消息助手，无依赖AsyncData，有请求应答机制
 */
class DefaultConnectorMessageHelper : public MessageHandler
{
public:
    DefaultConnectorMessageHelper();
    virtual             ~DefaultConnectorMessageHelper();

    // AsyncTask
public:
    virtual void        OnGetCreateAsyncData(AsyncDataList& asyncDataList);

    // MessageHelper
protected:
    // 真实处理消息
    virtual void        HandleMessage();

    // 处理数据超时
    virtual void        HandleDataTimeOut();

protected:
    // session应答指定请求
    void                SessionResponse(PlayerSession* session, CSRequest* request);
};


