#include "ShmObjectManager.h"
#include "ShmAllocator.h"
#include <assert.h>

ShmObjectManager::ShmObjectManager() : maxObjectID(0)
{
    mapShmObject.clear();
    mapShmObjectFactory.clear();
}

ShmObjectManager::~ShmObjectManager()
{
    maxObjectID = 0;
}

bool ShmObjectManager::Initialize()
{
    maxObjectID = 0;
    mapShmObject.clear();
    mapShmObjectFactory.clear();

    return true;
}

ShmObject* ShmObjectManager::CreateShmObject(WORD type)
{
    ShmObject* shmObject = NULL;

    ShmObjectFactoryMap::iterator iter = mapShmObjectFactory.find(type);
    if (iter == mapShmObjectFactory.end())
    {
        return NULL;
    }

    ShmObjectFactory* shmObjectFactory = iter->second;

    shmObject = shmObjectFactory->CreateShmObject(type);
    if (NULL == shmObject)
    {
        assert(false);
        return NULL;
    }
    
    shmObject->InitShmObject();
    
    shmObject->SetObjectID(GetNewObjectID());
    shmObject->SetObjectType(type);

    mapShmObject.insert(ShmObjectMap::value_type(shmObject->GetObjectID(), shmObject));

    return shmObject;
}

ShmObject* ShmObjectManager::FindShmObject(UINT objectID)
{
    ShmObjectMap::iterator iter = mapShmObject.find(objectID);
    if (iter == mapShmObject.end())
    {
        return NULL;
    }

    return iter->second;
}

void ShmObjectManager::ReleaseShmObject(ShmObject* object)
{
    if (object == NULL)
    {
        return;
    }
    
    mapShmObject.erase(object->GetObjectID());
    
    object->~ShmObject();
    
    ShmAllocator& shmAllot = ShmAllocator::Instance();
    shmAllot.Free(object->GetObjectAddr());
}

void ShmObjectManager::Revert()
{
    ShmChunkPool    chunkPool;
    ShmAllocator& shmAllot = ShmAllocator::Instance();
    
    if (!shmAllot.LoadShmChunk(chunkPool))
    {
        assert(false);
        return;
    }

    ShmObject* shmObject = NULL;

    while(1)
    {
        ShmChunk* chunk = chunkPool.GetChunk();
        if (NULL == chunk)
        {
            break;
        }

        ShmObjectFactoryMap::iterator iter = mapShmObjectFactory.find(chunk->type);
        if (iter == mapShmObjectFactory.end())
        {
            assert(false);
            return;
        }

        ShmObjectFactory* shmObjectFactory = iter->second;

        if (shmObjectFactory->GetShmObjectSize(chunk->type) != chunk->size)
        {
            assert(false && "size has been change");
            return;
        }

        shmObject = shmObjectFactory->RevertShmObject(chunk->type, chunk->data);
        if (NULL == shmObject)
        {
            assert(false);
            return;
        }
        
        mapShmObject.insert(ShmObjectMap::value_type(shmObject->GetObjectID(), shmObject));

        maxObjectID = (maxObjectID > shmObject->GetObjectID()) ? maxObjectID : shmObject->GetObjectID();
    }
}

UINT ShmObjectManager::GetNewObjectID()
{
    return ++maxObjectID;
}

void ShmObjectManager::RegisterShmObjectFactory(WORD type, ShmObjectFactory* shmObjectFactory)
{
    mapShmObjectFactory.insert(ShmObjectFactoryMap::value_type(type, shmObjectFactory));
}

void ShmObjectManager::PostRevert()
{
    for (ShmObjectMap::iterator iter = mapShmObject.begin(); iter != mapShmObject.end(); ++iter)
    {
        ShmObject* object = iter->second;
        if (object != NULL)
        {
            object->Revert();
        }
    }

//    for (ShmObjectMap::iterator iter = mapShmObject.begin(); iter != mapShmObject.end(); ++iter)
//    {
//        EventHandler* handler = dynamic_cast<EventHandler*>(iter->second);
//        if (handler != NULL)
//        {
//            handler->RegisterHandler();
//        }
//    }
}

