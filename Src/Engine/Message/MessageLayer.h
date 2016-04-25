/*
 * MessageLayer.h
 *
 *  Created on: 2015年11月10日
 *      Author: jarek
 */

#pragma once
#include <map>
#include <Language/DoubleQueue.h>
#include <Util/TMTSingleton.h>

class Message;
class MessageChannel;

/*
 * 消息层。维护消息管道，负责将消息交给对应管道处理。
 */
class MessageLayer : public TMTSingleton<MessageLayer>
{
public:
    MessageLayer();
    virtual             ~MessageLayer();

public:
    void                ReceiveAllMessage();

    void                HandleAllMessage();

    void                Send(Message& message, int remoteProcID);

    bool                AddChannel(MessageChannel& channel);

private:
    typedef std::map<int/*remoteProcID*/, MessageChannel*> ChannelMap;
    typedef DoubleQueue<Message*> MessageQueue;
private:
    ChannelMap          m_channelMap;
    MessageQueue        m_messageQueue;
};

