/*
 * ModuleCenter.cpp
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief:
 */

#include <Framework/ModuleCenter.h>
#include <Object/New.h>
#include <utility>


ModuleCenter::~ModuleCenter()
{
    for (TypeIDModuleMap::iterator iter = typeIDModuleMap.begin();
            iter != typeIDModuleMap.end(); ++iter)
    {
        Delete(iter->second);
    }
}

void ModuleCenter::StartGracefullyStop()
{
    for (LogicModuleList::iterator iter = logicModuleList.begin();
            iter != logicModuleList.end(); ++iter)
    {
        LogicModule* module = *iter;
        module->StartGracefullyStop();
    }
}

bool ModuleCenter::CheckGracefullyStopFinished()
{
    for (LogicModuleList::iterator iter = logicModuleList.begin();
            iter != logicModuleList.end(); ++iter)
    {
        LogicModule* module = *iter;
        if (!module->CheckGracefullyStopFinished())
        {
            return false;
        }
    }

    return true;
}

bool ModuleCenter::Initialize()
{
    InitLogicModule();
    InitConfigModule();

    for (ConfigModuleList::iterator iter = configModuleList.begin();
            iter != configModuleList.end(); ++iter)
    {
        if (!(*iter)->Initialize())
        {
            assert(false);
            return false;
        }
    }

    for (LogicModuleList::iterator iter = logicModuleList.begin();
            iter != logicModuleList.end(); ++iter)
    {
        if (!(*iter)->Initialize())
        {
            assert(false);
            return false;
        }
    }

    return true;
}

bool ModuleCenter::ReloadConfigModule()
{
    for (ConfigModuleList::iterator iter = configModuleList.begin();
            iter != configModuleList.end(); ++iter)
    {
        if (!(*iter)->Initialize())
        {
            assert(false);
            return false;
        }
    }

    return true;
}

Module* ModuleCenter::GetModule(UINT typeID)
{
    TypeIDModuleMap::iterator configIter = typeIDModuleMap.find(typeID);
    if (configIter != typeIDModuleMap.end())
    {
        return configIter->second;
    }

    assert(false);
    return NULL;
}

void ModuleCenter::AddModule(UINT typeID, ConfigModule* module)
{
    typeIDModuleMap[typeID] = module;
    configModuleList.push_back(module);
}

void ModuleCenter::AddModule(UINT typeID, LogicModule* module)
{
    typeIDModuleMap[typeID] = module;
    logicModuleList.push_back(module);
}
