/*
 * CSRequestMessage.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Log/LogMacro.h>
#include <Framework/GameServer.h>
#include <Message/Message/CSRequest.h>
#include <Message/Message/CSResponse.h>
#include <Message/MessageDefine.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Session/CSResponser.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>

CSRequest::CSRequest()
    : ProtoBuffMessage(MessageChannelType::Connector)
{
    // NULL
}

CSRequest::~CSRequest()
{
}

bool CSRequest::IsValid()
{
    if (GetMessageID() == CS_MSG_LOGIN_REQ)
    {
        return true;
    }

    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    PlayerSession* session = sessionModule.GetSessionBySessionID(GetSessionID());
    if (NULL == session)
    {
        // 无指定Session
        LOG_TRACE("Request<%s> session<ID:%d> is not found",
                BriefInfo(), GetSessionID());

        CSResponse response(*this);
        response.AddErrorMsg(MSG_ERROR_INVALID_SESSION);
        response.Send();
        return false;
    }

    if (!session->GetCSResponser().CheckRequestSequence(this))
    {
        // 序列号不对
        LOG_WARN("Request<%s> sequence is invalid, should be <%d>",
                BriefInfo(), session->GetCSResponser().GetValidRequestSequence());

        CSResponse response(*this);
        response.AddErrorMsg(MSG_ERROR_INVALID_SEQUENCE);
        response.Send();
        return false;
    }

    if (session->GetCSResponser().HaveRequest())
    {
        // 正在处理请求
        LOG_WARN("Request<%s> is dropped, former request is waiting for response", BriefInfo());

        CSResponse response(*this);
        response.AddErrorMsg(MSG_ERROR_TOO_MUCH_REQUEST);
        response.Send();
        return false;
    }

    return true;
}

int CSRequest::GetRequestID() const
{
    return protoBuffMsgSet.extra().cs_msg().request_id();
}



