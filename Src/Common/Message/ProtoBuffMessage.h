/*
 * ProtoBuffMessage.h
 *
 *  Created on: 2014年7月12日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Protocol/NetMsg.pb.h>
#include <Language/Visualizable.h>
#include <Engine/Message/Message/Message.h>

class ProtoBuffMessage : public Message
{
public:
    ProtoBuffMessage();
    virtual             ~ProtoBuffMessage();

    // Message
public:
    virtual bool        ParseMsg(const std::string& binaryData);

    virtual int         GetMessageID() const;

    virtual int         GetSequenceID() const;

    virtual int         GetSessionID() const;

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // 详细信息
    virtual std::string DetailInfo() const;

    // ProtoBuffMessage
public:
    // getter
    MsgSet&             GetMsgSet();
    const MsgSet&       GetMsgSet() const;

    // 一个MsgSet默认有一个msg，取出指定的msg
    NetMsg&             GetDefaultMsg();

    // 通过消息ID获取指定消息
    const NetMsg*       GetMsgByMsgID(MsgID msgID) const;

protected:
    MsgSet              protoBuffMsgSet;
};


