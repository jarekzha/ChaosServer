/*
 * IPCChannelManager.cpp
 *
 *  Created on: 2014年10月28日
 *  Author: jarek
 *  Brief: 
 */

#include <IPC/IPCChannelManager.h>
#include <IPC/IPCConfig.h>
#include <IPC/IPCDuplexChannel.h>
#include <IPC/IPCZMQTunnel.h>
#include <Log/LogMacro.h>
#include <Object/New.h>
#include <string>

using namespace std;

IPCChannelManager::IPCChannelManager()
{
    selfProcID = 0;
}

IPCChannelManager::~IPCChannelManager()
{
    for (ChannelMap::iterator iter = channelMap.begin(); iter != channelMap.end(); ++iter)
    {
        Delete(iter->second);
    }
}

bool IPCChannelManager::Initialize(const std::string& configFile, int selfProcID)
{
    if (!config.LoadFromXml(configFile.c_str()))
    {
        LOG_ERR("IPCChannelManager load config<%s> fail", configFile.c_str());
        return false;
    }

    this->selfProcID = selfProcID;

    for (size_t i = 0; i < config.Channel.size(); ++i)
    {
        const IPCChannelConfigItem& channelConfig = config.Channel[i];
        if (!channelConfig.CheckHaveProc(selfProcID))
        {
            continue;
        }

        IPCZMQTunnel* channel = New<IPCZMQTunnel>();
        if (!channel->Initialize(i, selfProcID, config.NamePrefix, channelConfig))
        {
            LOG_ERR("IPCChannelManager init channel fail");
            Delete(channel);
            return false;
        }

        channelMap[channel->GetRemoteProcID()] = channel;
    }

    recvIter = channelMap.begin();

    return true;
}

bool IPCChannelManager::SendData(int remoteProcID, const void* data, size_t dataLen)
{
    IPCZMQTunnel* channel = GetChannelByRemoteProcID(remoteProcID);
    if (NULL == channel)
    {
        LOG_ERR("IPCChannelManager send data fail, no channel<remoteProcID:%d> found", remoteProcID);
        return false;
    }

    return channel->SendMsg(data, dataLen);
}

bool IPCChannelManager::TryRecvData(int& remoteProcID, void* buffer, size_t& recvLen, size_t bufferLen)
{
    remoteProcID = 0;
    recvLen = 0;
    ASSERT_RETURN(buffer != NULL && bufferLen > 0, false);

    IPCZMQTunnel* channel = recvIter->second;
    ModifyRecvIter(recvIter);

    ASSERT_RETURN(channel != NULL, false);

    remoteProcID = channel->GetRemoteProcID();
    return channel->TryRecvMsg(buffer, recvLen, bufferLen);
}

bool IPCChannelManager::TryRecvDataFromRemote(int remoteProcID, void* buffer, size_t& recvLen, size_t bufferLen)
{
    IPCZMQTunnel* channel = GetChannelByRemoteProcID(remoteProcID);
    if (NULL == channel)
    {
        LOG_ERR("IPCChannelManager send data fail, no channel<remoteProcID:%d> found", remoteProcID);
        return false;
    }

    return channel->TryRecvMsg(buffer, recvLen, bufferLen);
}

IPCZMQTunnel* IPCChannelManager::GetChannelByRemoteProcID(int remoteProcID)
{
    ChannelMap::iterator iter = channelMap.find(remoteProcID);
    if (iter == channelMap.end())
    {
        return NULL;
    }

    return iter->second;
}

void IPCChannelManager::ModifyRecvIter(ChannelMap::iterator& iter)
{
    ++iter;
    if (iter == channelMap.end())
    {
        iter = channelMap.begin();
    }
}

