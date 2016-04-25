/*
 * MessageLayer.cpp
 *
 *  Created on: 2014年7月8日
 *      Author: liaojiayu
 */

#include <assert.h>
#include <Factory/Factory.h>
#include <Log/LogMacro.h>
#include <Message/Message/Message.h>
#include <Message/Channel/MessageChannel.h>
#include <Message/Handler/MessageHandler.h>
#include <Message/Handler/MessageHandlerFactory.h>
#include <Message/MessageLayer.h>
#include <Object/New.h>
#include <Util/Assert.h>
#include <string>
using namespace std;


MessageLayer::MessageLayer()
{
    // NULL
}

MessageLayer::~MessageLayer()
{
    for (ChannelMap::iterator iter = m_channelMap.begin(); iter != m_channelMap.end(); ++iter)
    {
        Delete(iter->second);
    }
}

void MessageLayer::ReceiveAllMessage()
{
    for (ChannelMap::iterator it = m_channelMap.begin(); it != m_channelMap.end(); it++)
    {
        Message* msg = it->second->ReceiveMsg();
        if (NULL == msg)
        {
            continue;
        }

        m_messageQueue.Push(msg);
    }
}

void MessageLayer::HandleAllMessage()
{
    while (!m_messageQueue.Empty())
    {
        Message* msg = m_messageQueue.Pop();

        ASSERT_CONTINUE(msg != NULL);
        MessageHandler* msgHandler = MessageHandlerFactory::Instance().Create(msg->GetMessageID());
        if (NULL == msgHandler)
        {
            LOG_ERR("No Handler is found for msg<%s>", msg->BriefInfo().c_str());
            continue;
        }

        msgHandler->Handle(*msg);
    }

    m_messageQueue.Switch();
}

void MessageLayer::Send(Message& message, int remoteProcID)
{
    ChannelMap::iterator iter = m_channelMap.find(remoteProcID);
    if (iter == m_channelMap.end())
    {
        // 未找到相应通道
        assert(false);
        return;
    }

    iter->second->Send(message);
}

bool MessageLayer::AddChannel(MessageChannel& channel)
{
    assert(m_channelMap.find(channel.GetRemoteProcID()) == m_channelMap.end());

    if (!channel.Initialize())
    {
        LOG_ERR("MessageChannel initialize fail");
        assert(false);
        return false;
    }

    m_channelMap[channel.GetRemoteProcID()] = &channel;
    return true;
}
