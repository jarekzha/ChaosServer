/*
 * ShmNew.h
 *
 *  Created on: 2014年7月12日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/Util.h>
#include <Engine/Language/TypeID.h>
#include <Engine/ShmObject/ShmObjectManager.h>
#include <Object/ObjectCounter.h>


template <typename T>
T* ShmNew(WORD objType)
{
    ShmObject* shmObj = ShmObjectManager::Instance().CreateShmObject(objType);
    ASSERT_RETURN(shmObj != NULL, false);

    T* realObj = dynamic_cast<T*>(shmObj);
    if (NULL == realObj)
    {
        ShmObjectManager::Instance().ReleaseShmObject(shmObj);
        assert(false);
        return NULL;
    }

    ObjectCounter::Instance().CountNewSingle(VariableTypeName(*shmObj).c_str(), ObjectMemType::Shm);

    return realObj;
}

template <typename T>
void ShmDelete(T* ptr)
{
    ObjectCounter::Instance().CountDeleteSingle(VariableTypeName(*ptr).c_str(), ObjectMemType::Shm);
    ShmObjectManager::Instance().ReleaseShmObject(ptr);

    ptr = NULL;
}


