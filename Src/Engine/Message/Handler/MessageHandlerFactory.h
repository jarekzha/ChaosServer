/*
 * MessageHandlerFactory.h
 *
 *  Created on: 2015年11月14日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

/**
 * 消息处理器工厂
 */
class MessageHandlerFactory
        : public Factory<MessageHandler>
        , public TMTSingleton<MessageHandlerFactory>
{

};

