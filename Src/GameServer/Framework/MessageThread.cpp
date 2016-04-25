/*
 * MessageThread.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Util/TMTSingleton.h>
#include <Framework/MessageThread.h>
#include <Engine/Message/MessageLayer.h>


MessageThread::MessageThread()
{
    // NULL
}

MessageThread::~MessageThread()
{
    // NULL
}

void MessageThread::Routine()
{
    static MessageLayer& msgLayer = MessageLayer::Instance();

    msgLayer.ReceiveAllMessage();
}



