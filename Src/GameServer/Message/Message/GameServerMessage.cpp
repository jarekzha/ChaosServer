/*
 * SSMessage.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Log/LogMacro.h>
#include <Framework/GameServer.h>
#include <Message/Message/GameServerMessage.h>
#include <Player/Player.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <stddef.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>
#include <Framework/GameServerModuleCenter.h>

GameServerMessage::GameServerMessage(PlayerSession& session)
{
    protoBuffMsgSet.mutable_head()->set_session_id(session.GetSessionID());
}

GameServerMessage::GameServerMessage(Player& player)
{
    protoBuffMsgSet.mutable_head()->set_session_id(player.GetSessionID());
}

GameServerMessage::GameServerMessage(int sessionID)
{
    protoBuffMsgSet.mutable_head()->set_session_id(sessionID);
}

GameServerMessage::GameServerMessage()
{
    // NULL
}

void GameServerMessage::Send()
{
    MessageLayer::Instance().Send(*this, remoteProcID);
}

bool GameServerMessage::IsValid()
{
    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    PlayerSession* session = sessionModule.GetSessionBySessionID(GetSessionID());
    if (NULL == session)
    {
        // 无指定Session
        LOG_WARN("Message<%s> session<ID:%d> is not found",
                BriefInfo(), GetSessionID());

        return false;
    }

    return true;
}

void GameServerMessage::SetErrorMsg(MsgErrorID errorID)
{
    Msg& defaultMsg = GetDefaultMsg();
    defaultMsg.set_id(CS_MSG_ERROR_NTF);
    defaultMsg.mutable_body()->mutable_cs_error_ntf()->set_error_id(errorID);
}

