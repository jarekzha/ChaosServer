/*
 * EventDispatcher.cpp
 *
 *  Created on: 2014年8月22日
 *  Author: jarek
 *  Brief:
 */

#include <Util/Util.h>
#include <Event/EventHandler.h>
#include <Event/EventDispatcher.h>
#include <set>

using namespace std;

EventDispatcher::EventDispatcher()
{
    // NULL
}

EventDispatcher::~EventDispatcher()
{
    UnregisterAllHandler();
}

void EventDispatcher::RegisterHandler(UINT eventID, EventHandler& handler)
{
    eventHandlerMultimap.insert(EventHandlerMultimap::value_type(eventID, &handler));
}

void EventDispatcher::UnregisterHandler(UINT eventID, EventHandler& handler)
{
    EventHandlerMultimap::iterator lowerBound = eventHandlerMultimap.lower_bound(eventID);
    EventHandlerMultimap::iterator upperBound = eventHandlerMultimap.upper_bound(eventID);
    for (EventHandlerMultimap::iterator iter = lowerBound; iter != upperBound; ++iter)
    {
        if (iter->second == &handler)
        {
            eventHandlerMultimap.erase(iter);
        }
    }

    return;
}

void EventDispatcher::UnregisterAllHandler()
{
    set<EventHandler*> eventHandlerSet;
    for (EventHandlerMultimap::iterator iter = eventHandlerMultimap.begin(); iter != eventHandlerMultimap.end(); ++iter)
    {
        eventHandlerSet.insert(iter->second);
    }

    for (set<EventHandler*>::iterator iter = eventHandlerSet.begin(); iter != eventHandlerSet.end(); ++iter)
    {
        (*iter)->UnregisterAllCallback();
    }
}
