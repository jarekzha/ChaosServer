/*
 * CSResponseMessage.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Protocol/Macro.pb.h>
#include <Engine/Message/Util/Sendable.h>

class CSRequest;
class Player;
class PlayerSession;

/*
 * 针对CSRequest的应答
 */
class CSResponse : public ProtoBuffMessage
                 , public Sendable
{
public:
    CSResponse(CSRequest& request);
    virtual             ~CSResponse();

    void                AddErrorMsg(MsgErrorID errorID);

    void                AddMsg(const NetMsg&msg);

    // Sendable
public:
    virtual void        Send();
};

/*
 * 回复单个Message，对应protobuf中的单个协议
 */
class CSSubResponse : public Sendable
{
public:
    CSSubResponse(PlayerSession& session);
    CSSubResponse(Player& player);
    virtual             ~CSSubResponse();

    // Sendable
public:
    virtual void        Send();

    // CSSubResponse
public:
    NetMsg&             GetMsg();

    // 设置为错误消息
    void                SetErrorMsg(MsgErrorID errorID);

    static void         SendErrorMsg(Player& player, MsgErrorID errorID);

private:
    void                InitResponse();

public:
    NetMsg              protoBuffMsg;
    PlayerSession*      playerSession;
};

