/*
 * IPCMessageChannel.cpp
 *
 *  Created on: 2014年7月17日
 *  Author: jarek
 *  Brief:
 */

#include <Common/Message/IPCMessageChannel.h>
#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/IPC/Define.h>
#include <Engine/IPC/IPCHandle.h>
#include <Engine/Language/TypeID.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Object/New.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Protocol/NetMsg.pb.h>
#include <stddef.h>
#include <cassert>
#include <sstream>


using namespace std;

IPCMessageChannel::IPCMessageChannel()
{
    binaryMsg.reserve(MSG_CACHE_SIZE);
}

IPCMessageChannel::~IPCMessageChannel()
{
    // NULL
}

bool IPCMessageChannel::Initialize()
{
    if (!InitConfig(config))
    {
        LOG_ERR("IPCMessageChannel init config fail");
        assert(false);
        return false;
    }

    InitProcID(config.LocalProcID, config.RemoteProcID);
    return true;
}

Message* IPCMessageChannel::ReceiveMsg()
{
    // 拉取数据
    size_t msgLen = 0;
    IPCHandleRes ipcRes = IPCHandle::Instance().GetCompleteDataFromProc(config.RemoteProcID,
            &binaryMsg[0], msgLen, binaryMsg.capacity());
    if (ipcRes != IPCHandleRes_Success)
    {
        return NULL;
    }

    binaryMsg.resize(msgLen);

    // 创建消息
    Message* msg = config.MsgCreatorFunc();
    if (!msg->ParseMsg(binaryMsg))
    {
        Delete(msg);
        return NULL;
    }

    return msg;
}

void IPCMessageChannel::Send(Message& message)
{
    ProtoBuffMessage* protoBuffMsg = dynamic_cast<ProtoBuffMessage*>(&message);
    if (NULL == protoBuffMsg)
    {
        LOG_ERR("Message <%s> is not supportted", VariableTypeName(message).c_str());
        return;
    }

    MsgSet& msgSet = protoBuffMsg->GetMsgSet();

#if MSG_TRACE_ON
    LOG_TRACE("Send protobuf Msg<%s> in channel<%s>", protoBuffMsg->BriefInfo(), this->BriefInfo());
#endif

    IPCHandle::Instance().SendMessage(config.RemoteProcID, msgSet);
}

string IPCMessageChannel::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "remoteProcID:" << config.RemoteProcID;

    return briefInfo.str();
}

