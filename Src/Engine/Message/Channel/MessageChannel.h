/*
 * MessageChannel.h
 *
 *  Created on: 2015年11月10日
 *      Author: jarek
 */

#pragma once

class Message;

/*
 * 消息通道
 * 定义不同消息通道的统一接口。子类负责在响应不同类型的消息通道中首发消息
 *
 * 子类调用InitProcID初始化进程信息
 */
class MessageChannel
{
public:
    MessageChannel();
    virtual             ~MessageChannel(){}

    // Interface
public:
    // 初始化
    virtual bool        Initialize() = 0;

    // 收包，需要轮询调用，单次可能并不会收到
    virtual Message*    ReceiveMsg() = 0;

    // 发包
    virtual void        Send(Message& message) = 0;

    // MessageChannel Base Info
public:
    int                 GetLocalProcID() const;
    int                 GetRemoteProcID() const;

protected:
    void                InitProcID(int localProcID, int remoteProcID);

private:
    int                 localProcID;
    int                 remoteProcID;
};

