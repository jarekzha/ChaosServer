///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ShmObjectManager - 所有OBJECT的创建入口和管理入口
//          edit by kcinz
//

#pragma once

#include <map>
#include "ShmObject/ShmObject.h"
#include "ShmObject/ShmObjectFactory.h"
#include "Util/TMTSingleton.h"


class ShmObjectManager : public TMTSingleton<ShmObjectManager>
{
public:
                        ShmObjectManager();
    virtual             ~ShmObjectManager();

    bool                Initialize();

    ShmObject*          CreateShmObject(WORD type);

    ShmObject*          FindShmObject(UINT objectID);

    void                ReleaseShmObject(ShmObject* object);

    void                Revert();

    void                RegisterShmObjectFactory(WORD type, ShmObjectFactory* shmObjectFactory);
    
    void                PostRevert();

private:    
    UINT                GetNewObjectID();

private:
    typedef std::map<UINT, ShmObject*>  ShmObjectMap;
    ShmObjectMap        mapShmObject;

    UINT                maxObjectID;

    typedef std::map<WORD, ShmObjectFactory*>  ShmObjectFactoryMap;
    ShmObjectFactoryMap mapShmObjectFactory;
};

#define SHM_NEW(object_type)               \
    ShmObjectManager::Instance().CreateShmObject(object_type)

#define SHM_DELETE(obj_ptr)                \
    ShmObjectManager::Instance().ReleaseShmObject(obj_ptr), obj_ptr = NULL

#define SHM_NEW_CLASS(type, type_value)    \
    dynamic_cast<type*>(ShmObjectManager::Instance().CreateShmObject(type_value))


