/*
 * EventCallback.h
 *
 *  Created on: 2014年8月22日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <Language/TypeID.h>

class IEventCallback
{
public:
    virtual             ~IEventCallback(){}

    virtual UINT        GetEventID() = 0;
};

/*
 * 事件回调函数
 */
template <typename Event>
class EventCallback : public IEventCallback
{
public:
    virtual             ~EventCallback(){}

    template<typename T>
    EventCallback(T* object, void(T::*func)(const Event&))
    {
        callback = boost::bind(func, object);
    }

    // EventCallback
public:
    virtual void        Invoke(const Event& event)
    {
        (callback)(event);
    }

    // IEventCallback
public:
    virtual UINT        GetEventID()
    {
        return TypeID<Event>::ID;
    }

private:
    typedef boost::function<void(const Event&)> CallbackFuncType;

private:
    CallbackFuncType    callback;
};

