/*
 * ObjectStatistician.cpp
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 
 */

#include <Log/LogMacro.h>
#include <Object/ObjectCounter.h>
#include <string.h>

using namespace std;


////////////////////////////// ObjectInfo /////////////////////////////////////
ObjectInfo::ObjectInfo()
{
    memset(NewSingleCount, 0, sizeof(NewSingleCount));
    memset(DeleteSingleCount, 0, sizeof(DeleteSingleCount));
    NewArrayCount = 0;
    DeleteArrayCount = 0;
}

int ObjectInfo::CurSingleCount(ObjectMemType::Enum memType)
{
    return NewSingleCount[memType] - DeleteSingleCount[memType];
}

int ObjectInfo::CurArrayCount()
{
    return NewArrayCount - DeleteArrayCount;
}


////////////////////////////// ObjectCounter //////////////////////////////////
ObjectCounter::ObjectCounter()
{
    state = true;
}

ObjectCounter::~ObjectCounter()
{
    // NULL
}

void ObjectCounter::ChangeState(bool on)
{
    state = on;
}

void ObjectCounter::CountNewSingle(const char* className, ObjectMemType::Enum memType)
{
    if (!state)
    {
        return;
    }

    ObjectInfo& objInfo = objectInfoMap[string(className)];
    objInfo.NewSingleCount[memType]++;
}

void ObjectCounter::CountDeleteSingle(const char* className, ObjectMemType::Enum memType)
{
    if (!state)
    {
        return;
    }

    ObjectInfo& objInfo = objectInfoMap[string(className)];
    objInfo.DeleteSingleCount[memType]++;
}

void ObjectCounter::CountNewArray(const char* className)
{
    if (!state)
    {
        return;
    }

    ObjectInfo& objInfo = objectInfoMap[string(className)];
    objInfo.NewArrayCount++;
}

void ObjectCounter::CountDeleteArray(const char* className)
{
    if (!state)
    {
        return;
    }

    ObjectInfo& objInfo = objectInfoMap[string(className)];
    objInfo.DeleteArrayCount++;
}

void ObjectCounter::Print()
{
    LOG_RUN("=======================ObjectCounter========================");
    for (ObjectInfoMap::iterator iter = objectInfoMap.begin();
            iter != objectInfoMap.end(); ++iter)
    {
        ObjectInfo& objInfo = iter->second;
        LOG_RUN("ObjectCounter class<name:%s> exist heap:%d shm:%d array:%d",
                iter->first.c_str(),
                objInfo.CurSingleCount(ObjectMemType::Heap),
                objInfo.CurSingleCount(ObjectMemType::Shm),
                objInfo.CurArrayCount());
    }

    LOG_RUN("=======================ObjectCounter========================");
}
