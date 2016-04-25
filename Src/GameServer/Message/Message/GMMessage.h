/*
 * GMMessage.h
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#ifndef GMMESSAGE_H_
#define GMMESSAGE_H_
#include <Message/MessageDefine.h>
#include <string>
#include "../../../Engine/Message/Message.h"

class GMMessage: public Message
{
public:
    GMMessage();
    virtual ~GMMessage();

    //Message
public:
    virtual bool InitMsg(OriginalData * data);  //将channle收到的数据进行初始化（例如解包）
    virtual int GetMessageID()  { return 0; };
    virtual int GetSessionID()  { return 0; };
    virtual int GetSequenceID()   { return 0; };
    virtual bool IsValid()      {return true;}       //对初始化后的数据进行校验 对于cs消息需要校验session id和sequence
    virtual int GetChannelType() { return MessageChannelType::GMConnector; }

public:
    virtual    void ReclaimSelf() {}

public:
    const std::string& GetHttpRequestContent() const;

public:
    std::string         httpRequestContent;
};

#endif /* GMMESSAGE_H_ */
