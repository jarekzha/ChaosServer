/*
 * EventHandler.cpp
 *
 *  Created on: 2014年8月22日
 *  Author: jarek
 *  Brief: 
 */

#include <Event/EventHandler.h>
#include <Event/EventDispatcher.h>

EventHandler::EventHandler()
{
    dispatcher = NULL;
}

EventHandler::~EventHandler()
{
    UnregisterAllCallback();
}

void EventHandler::Initialize(EventDispatcher& dispatcher)
{
    this->dispatcher = &dispatcher;
}

void EventHandler::DoDispatcherRegister(UINT eventID)
{
    dispatcher->RegisterHandler(eventID, *this);
}

void EventHandler::UnregisterAllCallback()
{
    for (EventIDCallbackMap::iterator iter = callbackMap.begin(); iter != callbackMap.end(); ++iter)
    {
        IEventCallback* callback = iter->second;
        dispatcher->UnregisterHandler(callback->GetEventID(), *this);

        Delete(iter->second);
    }
}
