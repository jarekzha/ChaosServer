/*
 * IPCHandle.h
 *
 *  Created on: 2014年7月17日
 *      Author: jarek
 */


#pragma once

#include <IPC/Define.h>
#include <IPC/IPCCounter.h>
#include <Util/TMTSingleton.h>
#include <Thread/ThreadMutex.h>
#include <google/protobuf/message.h>
#include <map>
#include <string>

class ShmIPC;
class FifoBuffer;
class IPCChannelManager;


enum IPCHandleRes
{
    IPCHandleRes_Error           = -1,   // 发生错误
    IPCHandleRes_Success         = 0,    // 成功
    IPCHandleRes_NoData          = 1,    // 没有数据
};

/*
 * IPC句柄，
 * 从配置的ipc通道中，提取消息或者通过配置的ipc通道发送消息
 */
class IPCHandle : public TMTSingleton<IPCHandle>
{
public:
    IPCHandle();
    virtual             ~IPCHandle();

    bool                Initialize(const std::string& ipcConfig, int localProcID);

    IPCHandleRes        GetCompleteMessage(google::protobuf::Message& msg);

    IPCHandleRes        GetCompleteDataFromProc(int remoteProcID, void* outBuffer, size_t& outLen, size_t outBufferSize);

    void                SendMessage(int remoteProcID, const google::protobuf::Message& msg);

    bool                SendData(int remoteProcID, const void* data, int dataLen);

    void                PrintInfo();

private:
    bool                NoneMutexSendData(int dstProcID, const void* data, int dataLen);

    void                DisplayMsg(const google::protobuf::Message& msgSet);

    IPCHandleRes        ParseMsgSetFromData(google::protobuf::Message& msgSet, const void* msgData, int msgSetLen);

private:
    int                 localProcID;
    IPCCounter          counter;
    IPCChannelManager*  channelMgr;
    ThreadMutex         mutex;
};

