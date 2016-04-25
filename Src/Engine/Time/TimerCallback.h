/*
 * TimerCallback.h
 *
 *  Created on: 2014年7月4日
 *  Author: jarek
 *  Brief: 定时器回调函数
 */

#pragma once
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <string>
#include <Language/TypeID.h>

/*
 * 定时器回调函数
 */
class TimerCallback
{
public:
    TimerCallback():objectPtr(NULL){}

    template<typename T>
    TimerCallback(T* object, void(T::*func)())
    {
        callback = boost::bind(func, object);
        className = TypeID<T>::Name;
        objectPtr = object;
    }

    void                Invoke()
    {
        (callback)();
    }

    const std::string&        GetClassName() const
    {
        return className;
    }
private:
    typedef boost::function<void()> CallbackFuncType;

private:
    CallbackFuncType    callback;
    std::string         className;
    void*               objectPtr;
};

