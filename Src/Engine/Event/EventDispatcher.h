/*
 * EventDispatcher.h
 *
 *  Created on: 2014年8月22日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <map>
#include <Language/TypeID.h>
#include <Event/EventHandler.h>

class EventDispatcher
{
public:
    EventDispatcher();
    virtual             ~EventDispatcher();

    // EventDispatcher
public:
    void                RegisterHandler(UINT eventID, EventHandler& handler);

    void                UnregisterHandler(UINT eventID, EventHandler& handler);

    template <typename Event>
    void                Dispatcher(const Event& event)
    {
        EventHandlerMultimap::iterator lowerBound = eventHandlerMultimap.lower_bound(TypeID<Event>::ID);
        EventHandlerMultimap::iterator upperBound = eventHandlerMultimap.upper_bound(TypeID<Event>::ID);
        for (EventHandlerMultimap::iterator iter = lowerBound; iter != upperBound; ++iter)
        {
            EventHandler* handler = iter->second;
            handler->Handle(event);
        }
    }

private:
    void                UnregisterAllHandler();

private:
    typedef std::multimap<UINT, EventHandler*> EventHandlerMultimap;

private:
    EventHandlerMultimap eventHandlerMultimap;
};


