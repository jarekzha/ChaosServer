/*
 * Message.h
 *
 *  Created on: 2015年11月10日
 *      Author: jarek
 */

#include <string>
#include <Language/Visualizable.h>

/**
 * 消息
 */
class Message : public Visualizable
{
public:
    Message() {}
    virtual             ~Message() {}

    // Message
public:
    // 从二进制数据中解析协议
    virtual bool        ParseMsg(const std::string& binaryData) = 0;

    // 协议ID
    virtual int         GetMessageID() const = 0;

    // 协议序列号
    virtual int         GetSequenceID() const = 0;

    // 会话ID
    virtual int         GetSessionID() const = 0;

};

