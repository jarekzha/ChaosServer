///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ShmObject的工厂基类
//          edit by kcinz
//

#pragma once

#include "Util/Util.h"
#include "ShmObject/ShmObject.h"

class ShmObjectFactory
{
public:
    virtual             ~ShmObjectFactory()    {}
    
    /*创建共享对象*/
    virtual ShmObject*  CreateShmObject(WORD objectType) = 0;

    /*回收共享对象*/
    virtual void        ReleaseShmObject(ShmObject* object) = 0;

    /*恢复共享对象*/
    virtual ShmObject*  RevertShmObject(WORD objectType, char* buffer) = 0;

    /*获取共享对象的大小*/
    virtual size_t      GetShmObjectSize(WORD objectType) = 0;
};

