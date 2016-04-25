/*
 * DefaultGMConnectorMessageHelper.h
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#pragma once

#include <Engine/AsyncData/AsyncTask.h>
#include <Engine/Message/Handler/MessageHandler.h>

/*
 * 默认GM消息助手，无依赖AsyncData，无请求应答机制
 */
class DefaultGMConnectorMessageHelper : public MessageHandler
{
public:
    DefaultGMConnectorMessageHelper();
    virtual ~DefaultGMConnectorMessageHelper();

    // AsyncTask
public:
    virtual void        OnGetCreateAsyncData(AsyncDataList& asyncDataList);

    // MessageHelper
protected:
    // 真实处理消息
    virtual void        HandleMessage();

    // 处理数据超时
    virtual void        HandleDataTimeOut();
};

