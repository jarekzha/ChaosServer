/*
 * EventHandler.h
 *
 *  Created on: 2014年8月22日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <map>
#include <Util/Util.h>
#include <Log/LogMacro.h>
#include <Event/EventCallback.h>
#include <Object/New.h>

class EventDispatcher;

/*
 * 事件处理者
 */
class EventHandler
{
public:
    EventHandler();
    virtual             ~EventHandler();

    // EventHandler
public:
    void                Initialize(EventDispatcher& dispatcher);

    template <typename Event>
    void                Handle(const Event& event)
    {
        EventCallback<Event>* callback = GetCallback<Event>();
        if (NULL == callback)
        {
            LOG_ERR("Event<name:%s> handle fail in handler<name:%>, no callback found",
                    TypeID<Event>::Name.c_str(), VariableTypeName(*this).c_str());
            assert(false);
            return;
        }

        callback->Invoke(event);
    }

    template <typename T, typename Event>
    void                RegisterCallback(void (T::*func)(const Event&))
    {
        ASSERT_RETURN_VOID(func != NULL && dispatcher != NULL);

        if (GetCallback<Event>() != NULL)
        {
            assert(false);
            return;
        }

        EventCallback<Event>* callback = New<EventCallback<Event> >(Me<T>(), func);
        callbackMap.insert(EventIDCallbackMap::value_type(callback->GetEventID(), callback));

        DoDispatcherRegister(TypeID<Event>::ID);
    }

    void                UnregisterAllCallback();

private:
    template <typename Event>
    EventCallback<Event>* GetCallback()
    {
        EventIDCallbackMap::iterator iter = callbackMap.find(TypeID<Event>::ID);
        if (iter == callbackMap.end())
        {
            return NULL;
        }

        EventCallback<Event>* callback = dynamic_cast<EventCallback<Event>*>(*iter);
        return callback;
    }

    template <typename T>
    T*                  Me()
    {
        return dynamic_cast<T*>(this);
    }

private:
    void DoDispatcherRegister(UINT eventID);

private:
    typedef std::map<UINT, IEventCallback*> EventIDCallbackMap;

private:
    EventDispatcher*    dispatcher;
    EventIDCallbackMap  callbackMap;
};

