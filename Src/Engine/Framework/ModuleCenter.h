/*
 * ModuleCenter.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Framework/Module.h>
#include <Language/TypeID.h>
#include <Log/LogMacro.h>
#include <stddef.h>
#include <Util/Util.h>
#include <Object/New.h>
#include <cassert>
#include <list>
#include <map>

class ModuleCenterExtendable
{
public:
    virtual ~ModuleCenterExtendable(){}

    virtual void        InitLogicModule() = 0;
    virtual void        InitConfigModule() = 0;
};


class ModuleCenter
        : public ModuleCenterExtendable
        , public GracefullyStopable
{
public:
    virtual             ~ModuleCenter();

    // GracefullyStopable
public:
    virtual void        StartGracefullyStop();

    virtual bool        CheckGracefullyStopFinished();

    // ModuleCenter
public:
    template<typename ModuleType>
    ModuleType&         Get();

    // 初始化所有模块
    bool                Initialize();

    // 重新初始化配置模块
    bool                ReloadConfigModule();

protected:
    template<typename ModuleType>
    void                AddModule();

private:
    Module*             GetModule(UINT typeID);
    void                AddModule(UINT typeID, ConfigModule* module);
    void                AddModule(UINT typeID, LogicModule* module);

private:
    typedef std::map<UINT, Module*> TypeIDModuleMap;    // typeID->Module
    typedef std::list<ConfigModule*>    ConfigModuleList;
    typedef std::list<LogicModule*>     LogicModuleList;

private:
    TypeIDModuleMap     typeIDModuleMap;
    ConfigModuleList    configModuleList;
    LogicModuleList     logicModuleList;
};

template<typename ModuleType>
inline ModuleType& ModuleCenter::Get()
{
    Module* module = GetModule(TypeID<ModuleType>::ID);
    if (NULL == module)
    {
        assert(false);
        LOG_ERR("GetModule %s fail", TypeID<ModuleType>::Name.c_str());
        ModuleType* newModule = New<ModuleType>();
        AddModule(TypeID<ModuleType>::ID, newModule);
        return *newModule;
    }

    return dynamic_cast<ModuleType&>(*module);
}

template<typename ModuleType>
inline void ModuleCenter::AddModule()
{
    AddModule(TypeID<ModuleType>::ID, New<ModuleType>());
}



