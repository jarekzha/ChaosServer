/*
 * CSLoginReqHelper.h
 *
 *  Created on: 2014年7月22日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/AsyncData/AsyncTask.h>
#include <Src/GameServer/Message/Helper/DefaultConnectorMessageHandler.h>



class CSLoginReqHelper : public DefaultConnectorMessageHelper
{
public:
    CSLoginReqHelper();
    virtual             ~CSLoginReqHelper();

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
