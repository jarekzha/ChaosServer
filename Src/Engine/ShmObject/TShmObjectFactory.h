#pragma once

#include <map>
#include "../util/xUtil.h"
#include "ShmAllocator.h"
#include "../xThread/xThreadMutex.h"
#include "ShmObjectManager.h"

/*声明对象创建函数*/
#define DECLARE_OBJECT_FUNC(classname, classtype) \
    ShmObject* Create##classname()\
    {\
        ShmAllocator& _shmAllot_ = ShmAllocator::Instance();\
        char* _buffer_ = (char*)(_shmAllot_.Malloc(sizeof(classname), classtype));\
        ShmObject* obj = new(_buffer_) classname();\
        obj->SetObjectAddr(_buffer_);\
        return obj;\
    }\
    ShmObject* Revert##classname(char* buffer)\
    {\
        ShmObject* obj = new(buffer) classname();\
        obj->SetObjectAddr(buffer);\
        return obj;\
    }

/*注册对象创建函数*/
#define REGISTER_OBJECT_FUNC(type, factory, classname) \
    _mapCreator.insert(CreatorMap::value_type(type, &factory::Create##classname));\
    _mapRevert.insert(RevertMap::value_type(type, &factory::Revert##classname));\
    _mapSize.insert(SizeMap::value_type(type, sizeof(classname)));\
    ShmObjectManager::Instance().RegisterShmObjectFactory(type, this);


/*共享对象的创建和回收工厂模版类，用于分类创建和回收不同的共享对象*/
template <typename ConcreteFactory>
class TShmObjectFactory : public ShmObjectFactory
{
public:
                        TShmObjectFactory()     {mapCreator.clear();}
    virtual             ~TShmObjectFactory()    {mapCreator.clear();}

public:
    virtual ShmObject*  CreateShmObject(WORD objectType)
    {
        ThreadMutexGuard lock(mutex);
        
        CreatorMapIter iter = mapCreator.find(objectType);
        if (iter == mapCreator.end())
        {
            return NULL;
        }

        return (Me()->*(iter->second))();
    }

    virtual void        ReleaseShmObject(ShmObject* shmObject)
    {
        ThreadMutexGuard lock(mutex);
        
        ShmAllocator& shmAllot = ShmAllocator::Instance();

        shmAllot.Free(shmObject);
    }

    virtual ShmObject*  RevertShmObject(WORD objectType, char* buffer)
    {
        RevertMapIter iter = mapRevert.find(objectType);
        if (iter == mapRevert.end())
        {
            return NULL;
        }

        return (Me()->*(iter->second))(buffer);
    }

    virtual size_t      GetShmObjectSize(WORD objectType)
    {
        SizeMapIter iter = mapSize.find(objectType);
        if (iter == mapSize.end())
        {
            return 0;
        }
        
        return iter->second;
    }

private:
    ConcreteFactory*    Me()
    {
        return dynamic_cast<ConcreteFactory*>(this);
    }

protected:
    typedef ShmObject* (ConcreteFactory::*CreatorFunc)();
    typedef std::map<WORD, CreatorFunc>     CreatorMap;
    typedef typename CreatorMap::iterator   CreatorMapIter;

    CreatorMap          mapCreator;

    typedef ShmObject* (ConcreteFactory::*RevertFunc)(char*);
    typedef std::map<WORD, RevertFunc>      RevertMap;
    typedef typename RevertMap::iterator    RevertMapIter;

    RevertMap           mapRevert;

    typedef std::map<WORD, size_t>          SizeMap;
    typedef typename SizeMap::iterator      SizeMapIter;

    SizeMap             mapSize;

    ThreadMutex        mutex;
};

