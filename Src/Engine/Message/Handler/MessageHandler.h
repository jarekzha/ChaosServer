/*
 * MessageHandler.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <AsyncData/AsyncTask.h>
#include <Time/Timer.h>

class Message;

/*
 * 消息处理器，自动注册
 *
 *         子类需要实现如下函数
 *         virtual void        OnExecute() = 0;
 *         virtual void        OnGetCreateAsyncData(AsyncDataList& asyncDataList) = 0;
 */
class MessageHandler : private AsyncTask
{
public:
    MessageHandler();
    virtual             ~MessageHandler();

    // MessageHandler
public:
    // 开始处理消息
    virtual void        Handle(Message& msg);

    // AsyncTask
protected:
    virtual void        OnGetDataTimeOut();

    virtual void        OnRelease();

private:
    // Timer handleTimer的回调
    void                OnTimerHandle();

protected:
    Message*            message;

private:
    Timer               handleTimer;
};


