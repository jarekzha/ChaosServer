/*
 * MessageHandlerAutoRegister.h
 *
 *  Created on: 2015年11月15日
 *  Author: jarek
 *  Brief: 
 */

#pragma once


template <typename Handler, int MsgID>
class MessageHandlerAutoRegisterTool
{
public:
    MessageHandlerAutoRegisterTool()
    {
        MessageHandlerFactory::Instance().RegisterProduct<Handler>(MsgID);
    }
};

/**
 * 消息处理器自动注册器
 */
template <typename Handler, int MsgID>
class MessageHandlerAutoRegister : public MessageHandler
{
private:
    static MessageHandlerAutoRegisterTool<Handler, MsgID> registerTool;
};

template <typename Handler, int MsgID>
MessageHandlerAutoRegisterTool<Handler, MsgID> MessageHandlerAutoRegister<Handler, MsgID>::registerTool;

