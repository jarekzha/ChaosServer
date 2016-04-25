/*
 * IPCMessageChannel.h
 *
 *  Created on: 2014年7月17日
 *  Author: jarek
 *  Brief: 比较通用的使用ShmIPC的message channel
 */

#pragma once

#include <Language/Visualizable.h>
#include <Message/Channel/MessageChannel.h>
#include <string>

class IPCHandle;
class FifoBuffer;
class ProtoBuffMessage;

// ProtoBuffMessage创建函数类型
typedef ProtoBuffMessage* (*ProtoBufMsgCreator)();

/**
 * 进程间消息通道
 */
class IPCMessageChannel
        : public MessageChannel
        , public Visualizable
{
public:
    IPCMessageChannel();
    virtual             ~IPCMessageChannel();

    // MessageChannel
public:
    virtual bool        Initialize();

    virtual Message*    ReceiveMsg();

    virtual void        Send(Message& message);

    // Visualizable
public:
    virtual std::string BriefInfo() const;

protected:
    struct Config
    {
        Config(): LocalProcID(0), RemoteProcID(0), MsgCreatorFunc(NULL){}

        int             RemoteProcID;
        int             LocalProcID;
        ProtoBufMsgCreator  MsgCreatorFunc;
    };

    virtual bool        InitConfig(Config& config) = 0;

private:
    Config              config;
    std::string         binaryMsg;
};


