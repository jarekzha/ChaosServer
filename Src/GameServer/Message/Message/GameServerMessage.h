/*
 * SSMessage.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Protocol/Macro.pb.h>

class PlayerSession;
class Player;


class GameServerMessage
        : public ProtoBuffMessage
        , public Sendable
{
public:
    GameServerMessage(PlayerSession& session);
    GameServerMessage(Player& player);
    GameServerMessage(int sessionID);
    GameServerMessage();
    virtual             ~GameServerMessage(){}

    // Message
public:
    virtual bool        IsValid();

    // GameServerMessage
public:
    // 设置为错误消息
    void                SetErrorMsg(MsgErrorID errorID);

    // Sendable
public:
    virtual void        Send();
};
