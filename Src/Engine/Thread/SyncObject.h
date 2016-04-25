///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  同步对象基类
//          edit by kcinz
//
#pragma once

class SyncObject
{
public:
    virtual             ~SyncObject(){}

public:
    virtual             int Lock() = 0;
    virtual             int Unlock() = 0;
    virtual             int TryLock() = 0;
};


