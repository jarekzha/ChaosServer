/*
 * CSResponse.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Message/Message/CSRequest.h>
#include <Message/Message/CSResponse.h>
#include <Player/Player.h>
#include <Protocol/NetMsg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Session/CSResponser.h>
#include <Session/PlayerSession.h>
#include <Engine/Time/Time.h>
#include <Engine/Message/MessageLayer.h>


/////////////////////////////////// CSResponse ////////////////////////////////
CSResponse::CSResponse(CSRequest& request)
    : ProtoBuffMessage(MessageChannelType::Connector)
{
    protoBuffMsgSet.mutable_head()->set_session_id(request.GetSessionID());
    protoBuffMsgSet.mutable_head()->set_svrtime(Time::GetCurSecond());
    protoBuffMsgSet.mutable_head()->set_sequence(request.GetSequenceID() + 1);
    protoBuffMsgSet.mutable_extra()->mutable_cs_msg()->set_request_id(request.GetRequestID());
}

CSResponse::~CSResponse()
{
    // NULL
}

void CSResponse::AddErrorMsg(MsgErrorID errorID)
{
    NetMsg* newMsg = protoBuffMsgSet.mutable_msg()->Add();

    newMsg->set_id(CS_MSG_ERROR_NTF);
    newMsg->mutable_body()->mutable_cs_error_ntf()->set_error_id(errorID);
}

void CSResponse::AddMsg(const NetMsg& msg)
{
    *protoBuffMsgSet.mutable_msg()->Add() = msg;
}

void CSResponse::Send()
{
    theGameServer.GetMessageLayer().Send(this);
}

/////////////////////////////////// CSSubResponse /////////////////////////////
CSSubResponse::CSSubResponse(PlayerSession& session)
{
    InitResponse();

    playerSession = &session;
}

CSSubResponse::CSSubResponse(Player& player)
{
    InitResponse();

    playerSession = player.GetSession();
    if (NULL == playerSession)
    {
        LOG_ERR("CSSubResponse construct with player fail no sesion found");
    }
}

CSSubResponse::~CSSubResponse()
{
}

void CSSubResponse::Send()
{
    ASSERT_RETURN_VOID(playerSession != NULL);

    if (!protoBuffMsg.IsInitialized())
    {
        LOG_ERR("CSSubResponse send fail, msg is not initialized");
        assert(false);
        return;
    }

    playerSession->GetCSResponser().AddSubResponse(protoBuffMsg);
}

Msg& CSSubResponse::GetMsg()
{
    return protoBuffMsg;
}

void CSSubResponse::SetErrorMsg(MsgErrorID errorID)
{
    protoBuffMsg.set_id(CS_MSG_ERROR_NTF);
    protoBuffMsg.mutable_body()->mutable_cs_error_ntf()->set_error_id(errorID);
}

void CSSubResponse::SendErrorMsg(Player& player, MsgErrorID errorID)
{
    CSSubResponse response(player);
    response.SetErrorMsg(errorID);
    response.Send();
}

void CSSubResponse::InitResponse()
{
    playerSession = NULL;
}








