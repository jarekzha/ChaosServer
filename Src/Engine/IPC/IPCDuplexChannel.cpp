/*
 * IPCDuplexChannel.cpp
 *
 *  Created on: 2014年10月28日
 *  Author: jarek
 *  Brief: 
 */

#include <boost/interprocess/creation_tags.hpp>
#include <IPC/Define.h>
#include <IPC/IPCConfig.h>
#include <IPC/IPCDuplexChannel.h>
#include <Log/LogMacro.h>
#include <Object/New4.h>
#include <stddef.h>
#include <Util/Assert.h>
#include <Util/Util.h>
#include <exception>
#include <sstream>

using namespace std;
using namespace boost::interprocess;

IPCDuplexChannel::IPCDuplexChannel()
{
    config = NULL;
    index = 0;
    selfProcID = 0;
    remoteProcID = 0;
    sendQueue = NULL;
    recvQueue = NULL;
}

IPCDuplexChannel::~IPCDuplexChannel()
{
    SAFE_DELETE(sendQueue);
    SAFE_DELETE(recvQueue);
}

string IPCDuplexChannel::BriefInfo() const
{
    stringstream briefInfo;

    briefInfo << "remoteProcID:" << remoteProcID;
    return briefInfo.str();
}

bool IPCDuplexChannel::Initialize(int index, int selfProcID, const std::string& namePrefix, const IPCChannelConfigItem& config)
{
    ASSERT_RETURN(index >= 0 && selfProcID >= 0, false);

    this->config = &config;
    this->index = index;
    this->selfProcID = selfProcID;

    int sendQueueSize = 0;
    int recvQueueSize = 0;
    string sendQueueName;
    string recvQueueName;
    if (config.Proc1 == selfProcID)
    {
        remoteProcID = config.Proc2;
        sendQueueSize = config.Channel1to2Size;
        recvQueueSize = config.Channel2to1Size;
        sendQueueName = IPCChannelName(namePrefix, index, config, IPCSubChannelType::Proc1to2);
        recvQueueName = IPCChannelName(namePrefix, index, config, IPCSubChannelType::Proc2to1);
    }
    else if (config.Proc2 == selfProcID)
    {
        remoteProcID = config.Proc1;
        sendQueueSize = config.Channel2to1Size;
        recvQueueSize = config.Channel1to2Size;
        sendQueueName = IPCChannelName(namePrefix, index, config, IPCSubChannelType::Proc2to1);
        recvQueueName = IPCChannelName(namePrefix, index, config, IPCSubChannelType::Proc1to2);
    }
    else
    {
        LOG_ERR("IPChannel<%s> init fail, invalid config", BriefInfo().c_str());
        return false;
    }


    sendQueue = New<MessageQueue>(open_or_create, sendQueueName.c_str(), sendQueueSize, MSG_CACHE_SIZE);
    recvQueue = New<MessageQueue>(open_or_create, recvQueueName.c_str(), recvQueueSize, MSG_CACHE_SIZE);

    return true;
}

bool IPCDuplexChannel::SendMsg(const void* data, size_t dataLen)
{
    ASSERT_RETURN(data != NULL && dataLen > 0, false);

    bool result = true;
    try
    {
        result = sendQueue->try_send(data, dataLen, 0);
    }
    catch (exception& e)
    {
        LOG_ERR("IPCDuplexChannel<%s> send msg<size:%d> fail<%s>",
                BriefInfo().c_str(), (int)dataLen, e.what());
        return false;
    }

    if (!result)
    {
        LOG_ERR("IPCDuplexChannel<%s> send msg<size:%d> fail, maybe channel is full",
                BriefInfo().c_str(), (int)dataLen);
        return false;
    }

    return true;
}

bool IPCDuplexChannel::TryRecvMsg(void* buffer, size_t& recvLen, size_t bufferLen)
{
    ASSERT_RETURN(buffer != NULL && bufferLen > 0, false);

    recvLen = 0;

    try
    {
        unsigned int priority = 0;
        recvQueue->try_receive(buffer, bufferLen, recvLen, priority);
    }
    catch (exception& e)
    {
        LOG_ERR("IPCDuplexChannel<%s> try recv msg fail<%s>",
                BriefInfo().c_str(), e.what());
        return false;
    }

    return true;
}

int IPCDuplexChannel::GetRemoteProcID() const
{
    return remoteProcID;
}



