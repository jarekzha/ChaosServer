///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ShmObject - 所有共享内存子类的基类
//          edit by kcinz
//

#pragma once

#include "Util/Util.h"

class ShmObject
{
    UINT                objectID;
    WORD                objectType;
    void*               objectAddr;

public:
                        ShmObject()     {}
    virtual             ~ShmObject()    {objectID = 0; }

    void                SetObjectID(UINT objectID)      {this->objectID = objectID;}
    UINT                GetObjectID()                   {return objectID;}

    void                SetObjectType(WORD objectType)  {this->objectType = objectType;}
    WORD                GetObjectType()                 {return objectType;}

    void                SetObjectAddr(void* objectAddr) {this->objectAddr = objectAddr;}
    void*               GetObjectAddr()                 {return objectAddr;}

    bool                InitShmObject()                 {objectID = 0; objectType = 0; return Init();}

    /*object 恢复创建之后调用*/
    virtual void        Revert() = 0;

    /*object 全新创建好之后调用*/
    virtual bool        Init() = 0;
};



