#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtrManager.h"
#include "ShmObject/SIShmAllocator.h"

ShmContainerManager::ShmContainerManager()
{
    maxID = 0;
    listRevertFunc.clear();
    initFlag = false;
}

ShmContainerManager::~ShmContainerManager()
{
}

bool ShmContainerManager::Initialize()
{
    IDPtrManager& idPtrManager = IDPtrManager::Instance();
    idPtrManager.Initialize();
    
    SIShmAllocator& xsiShmAllot = SIShmAllocator::Instance();
    xsiShmAllot.Initialize();

    return true;
}

UINT ShmContainerManager::GetNewContainerID()
{
    return ++maxID;
}

void ShmContainerManager::RegisterRevertFunc(RevertFunc& func)
{
    listRevertFunc.push_back(func);
}

void ShmContainerManager::RecoverID(UINT containerID)
{
    maxID = (maxID > containerID) ? maxID : containerID;
}

void ShmContainerManager::Revert()
{
    IDPtrManager& idPtrManager = IDPtrManager::Instance();
    idPtrManager.Revert();
    
    RevertFuncList::iterator iter = listRevertFunc.begin();

    for (; iter != listRevertFunc.end(); ++iter)
    {
        RevertFunc& func = *iter;
        func();
    }

    initFlag = true;
}

bool ShmContainerManager::NeedInit()
{
    return initFlag;
}

