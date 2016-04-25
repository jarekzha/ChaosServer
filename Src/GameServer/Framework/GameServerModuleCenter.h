/*
 * GameServerModuleCenter.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Framework/ModuleCenter.h>
#include <Engine/Util/TMTSingleton.h>

/**
 * 游戏逻辑服务器模块中心
 */
class GameServerModuleCenter
        : public ModuleCenter
        , public TMTSingleton<GameServerModuleCenter>
{
    // ModuleCenterExtendable
public:
    virtual void        InitLogicModule();
    virtual void        InitConfigModule();
};


#define theModule(Module)   GameServerModuleCenter::Instance().Get<Module>()
