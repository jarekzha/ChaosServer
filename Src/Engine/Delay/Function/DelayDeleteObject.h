/*
 * DelayDeleteObject.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Delay/DelayFunction.h>
#include <Object/New.h>

/*
 * 延迟删除各种对象，通过其指针
 */
template <typename T>
class DelayDeleteObject : public DelayFunction
{
public:
    DelayDeleteObject(T* obj)
    {
        this->object = obj;
    }

public:
    virtual void        Invoke()
    {
        Delete(object);
    }

private:
    T*                  object;
};
