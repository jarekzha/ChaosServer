#include "ShmObject/IDPtrManager.h"

void IDPtrManager::RegisterIDPtrFunc(RevertFunc& func)
{
    listFunc.push_back(func);
}

void IDPtrManager::Revert()
{
    FuncList::iterator iter = listFunc.begin();
    for (; iter != listFunc.end(); ++iter)
    {
        (*iter)();
    }

    initFlag = true;
}

