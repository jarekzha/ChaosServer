/*
 * IPCDuplexChannel.h
 *
 *  Created on: 2014年10月28日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Language/Visualizable.h>
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>

class IPCChannelConfigItem;
class message_queue;

/**
 * 用于IPC通信的双工通道
 */
class IPCDuplexChannel : public Visualizable
{
public:
    IPCDuplexChannel();
    virtual             ~IPCDuplexChannel();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

public:
    bool                Initialize(int index,
                                   int selfProcID,
                                   const std::string& namePrefix,
                                   const IPCChannelConfigItem& config);

    bool                SendMsg(const void* data, size_t dataLen);

    bool                TryRecvMsg(void* buffer, size_t& recvLen, size_t bufferLen);

    int                 GetRemoteProcID() const;

private:
    typedef boost::interprocess::message_queue MessageQueue;

private:
    const IPCChannelConfigItem* config;

    int                 index;
    int                 selfProcID;
    int                 remoteProcID;
    MessageQueue*       sendQueue;
    MessageQueue*       recvQueue;
};

