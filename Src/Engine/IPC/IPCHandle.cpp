/*
 * IPCHandle.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: jarek
 */

#include <IPC/Define.h>
#include <IPC/IPCChannelManager.h>
#include <IPC/IPCHandle.h>
#include <Log/LogMacro.h>
#include <Object/New.h>
#include <Object/New0.h>
#include <stddef.h>
#include <Util/Assert.h>

using namespace std;
using namespace google::protobuf;


IPCHandle::IPCHandle()
{
    localProcID = 0;
    channelMgr = NULL;
}

IPCHandle::~IPCHandle()
{
    Delete(channelMgr);
}

bool IPCHandle::Initialize(const std::string& ipcConfig, int localProcID)
{
    this->localProcID = localProcID;
    channelMgr = New<IPCChannelManager>();

    if (!channelMgr->Initialize(ipcConfig, localProcID))
    {
        return false;
    }
    
    return true;
}

IPCHandleRes IPCHandle::GetCompleteMessage(::google::protobuf::Message& msg)
{
    ThreadMutexGuard lock(mutex);

    char msgData[MSG_CACHE_SIZE];
    size_t msgSetLen = 0;
    int remoteProcID = 0;

    bool result = channelMgr->TryRecvData(remoteProcID, msgData, msgSetLen, sizeof(msgData));
    if (!result)
    {
        return IPCHandleRes_Error;
    }

    if (msgSetLen == 0)
    {
        return IPCHandleRes_NoData;
    }

    return ParseMsgSetFromData(msg, msgData, msgSetLen);
}

IPCHandleRes IPCHandle::GetCompleteDataFromProc(int remoteProcID, void* outBuffer, size_t& outLen, size_t outBufferSize)
{
    ASSERT_RETURN(remoteProcID > 0, IPCHandleRes_Error);

    ThreadMutexGuard lock(mutex);

    bool result = channelMgr->TryRecvDataFromRemote(remoteProcID, outBuffer, outLen, outBufferSize);
    if (!result)
    {
        return IPCHandleRes_Error;
    }

    if (outLen == 0)
    {
        return IPCHandleRes_NoData;
    }

    return IPCHandleRes_Success;
}

void IPCHandle::SendMessage(int remoteProcID, const google::protobuf::Message& msg)
{
    ThreadMutexGuard lock(mutex);

    char sendData[MSG_CACHE_SIZE];
    size_t sendLen = msg.ByteSize();
    if (!msg.SerializeToArray(sendData, sizeof(sendData)))
    {
        LOG_ERR("Message serialize fail <%s>", msg.DebugString().c_str());
        return;
    }

    if (!NoneMutexSendData(remoteProcID, sendData, sendLen))
    {
        LOG_ERR("Message send fail <%s>", msg.DebugString().c_str());
        return;
    }
}

void IPCHandle::PrintInfo()
{
    LOG_RUN("=======================IPCHandle========================");
    counter.Print(localProcID);
    LOG_RUN("=======================IPCHandle========================");
}

bool IPCHandle::SendData(int remoteProcID, const void* data, int dataLen)
{
    ThreadMutexGuard lock(mutex);

    return NoneMutexSendData(remoteProcID, data, dataLen);
}

bool IPCHandle::NoneMutexSendData(int remoteProcID, const void* data, int dataLen)
{
    bool ret = channelMgr->SendData(remoteProcID, data, dataLen);
    if (!ret)
    {
        return false;
    }

    counter.CountSend(remoteProcID, dataLen);
    return true;
}

IPCHandleRes IPCHandle::ParseMsgSetFromData(google::protobuf::Message& msg, const void* msgData, int msgSetLen)
{
    if (!msg.ParseFromArray(msgData, msgSetLen))
    {
        LOG_ERR("Parse msg error");
        return IPCHandleRes_Error;
    }

    return IPCHandleRes_Success;
}

void IPCHandle::DisplayMsg(const google::protobuf::Message& msg)
{
    LOG_RUN("------> visualized msg: %s", msg.DebugString().c_str());
}

