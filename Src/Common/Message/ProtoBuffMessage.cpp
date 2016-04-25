/*
 * ProtoBuffMessage.cpp
 *
 *  Created on: 2014年7月12日
 *  Author: jarek
 *  Brief:
 */

#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/Log/LogMacro.h>
#include <sstream>
#include <Engine/Language/String.h>
#include <Engine/Util/Util.h>
using namespace std;

ProtoBuffMessage::ProtoBuffMessage()
{
    // NULL
}

ProtoBuffMessage::~ProtoBuffMessage()
{
    // NULL;
}

bool ProtoBuffMessage::ParseMsg(const string& binaryData)
{
    if (!protoBuffMsgSet.ParseFromString(binaryData))
    {
        LOG_ERR("Parse msg error data<%s>", BinaryToReadableString(binaryData.c_str(), binaryData.size()).c_str());
        assert(false);
        return false;
    }

#if MSG_TRACE_ON
    LOG_TRACE("ProtobufMsg<%s> init from orignial data to be handled",
            BriefInfo());
#endif

    return true;
}

int ProtoBuffMessage::GetMessageID() const
{
    return protoBuffMsgSet.msg().Get(0).id();
}

int ProtoBuffMessage::GetSequenceID() const
{
    return protoBuffMsgSet.head().sequence();
}

string ProtoBuffMessage::BriefInfo() const
{
    stringstream briefInfo;

    briefInfo << "sessionID:" << protoBuffMsgSet.head().session_id();
    briefInfo << ", sequence:" << protoBuffMsgSet.head().sequence();
    for (int i = 0; i < protoBuffMsgSet.msg().size(); ++i)
    {
        briefInfo << ", msgID:" << MsgID_Name(protoBuffMsgSet.msg().Get(i).id());
    }

    return briefInfo.str();
}

string ProtoBuffMessage::DetailInfo() const
{
    return protoBuffMsgSet.DebugString();
}

MsgSet& ProtoBuffMessage::GetMsgSet()
{
    return protoBuffMsgSet;
}

const MsgSet& ProtoBuffMessage::GetMsgSet() const
{
    return protoBuffMsgSet;
}

int ProtoBuffMessage::GetSessionID() const
{
    return protoBuffMsgSet.head().session_id();
}

NetMsg& ProtoBuffMessage::GetDefaultMsg()
{
    if (protoBuffMsgSet.msg().size() == 0)
    {
        return *(protoBuffMsgSet.mutable_msg()->Add());
    }
    else
    {
        return *(protoBuffMsgSet.mutable_msg()->Mutable(0));
    }
}

const NetMsg* ProtoBuffMessage::GetMsgByMsgID(MsgID msgID) const
{
    for (int i = 0; i < protoBuffMsgSet.msg().size(); ++i)
    {
        const NetMsg& msg = protoBuffMsgSet.msg(i);
        if (msg.id() == msgID)
        {
            return &msg;
        }
    }

    return NULL;
}


