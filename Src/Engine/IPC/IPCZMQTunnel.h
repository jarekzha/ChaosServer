/*
 * IPCZMQTunnel.h
 *
 *  Created on: 2015年11月11日
 *  Author: seekerwang
 *  Brief: 
 */

#pragma once
#include <Language/Visualizable.h>
#include <string>

class IPCChannelConfigItem;
class IPCZMQTunnel : public Visualizable
{
public:
    IPCZMQTunnel();
    virtual             ~IPCZMQTunnel();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

public:
    bool                Initialize(int index,
                                   int selfProcID,
                                   const std::string& ipcPath,
                                   const IPCChannelConfigItem& config);

    bool                SendMsg(const void* data, size_t dataLen);

    bool                TryRecvMsg(void* buffer, size_t& recvLen, size_t bufferLen);

    int                 GetRemoteProcID() const;

private:
    const IPCChannelConfigItem* config;

    int                 index;
    int                 selfProcID;
    int                 remoteProcID;

    void*               zmqContext;
    void*               zmqSocket;
};

