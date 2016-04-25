/*
 * DefaultMessageHelper.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/AsyncData/AsyncTask.h>
#include <Engine/Message/Handler/MessageHandler.h>

/*
 * 默认消息助手，无依赖AsyncData，无请求应答机制
 */
class DefaultMessageHelper : public MessageHandler
{
public:
    DefaultMessageHelper();
    virtual             ~DefaultMessageHelper();

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


