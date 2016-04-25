/*
 * IPCChannelManager.h
 *
 *  Created on: 2014年10月28日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <IPC/IPCConfig.h>
#include <map>
#include <string>

class IPCZMQTunnel;
/**
 * IPC通道管理器
 */
class IPCChannelManager
{
private:
    typedef std::map<int, IPCZMQTunnel*> ChannelMap;

public:
    IPCChannelManager();
    virtual             ~IPCChannelManager();

public:
    bool                Initialize(const std::string& configFile, int selfProcID);

    bool                SendData(int remoteProcID, const void* data, size_t dataLen);

    bool                TryRecvData(int& remoteProcID, void* buffer, size_t& recvLen, size_t bufferLen);

    bool                TryRecvDataFromRemote(int remoteProcID, void* buffer, size_t& recvLen, size_t bufferLen);

private:
    IPCZMQTunnel*       GetChannelByRemoteProcID(int remoteProcID);

    void                ModifyRecvIter(ChannelMap::iterator& iter);

private:
    IPCConfigFile       config;
    ChannelMap          channelMap;
    ChannelMap::iterator recvIter;          // 用于遍历channel收消息的迭代器
    int                 selfProcID;         // 所在进程的procID
};

