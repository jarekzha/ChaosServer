/*
 * GameServerModuleCenter.cpp
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#include <Framework/GameServerModuleCenter.h>
#include <GameMaster/GameMasterModule.h>
#include <Player/PlayerModule.h>
#include <Prop/Config/PropConfigModule.h>
#include <Prop/PropModule.h>
#include <Session/PlayerSessionModule.h>

void GameServerModuleCenter::InitLogicModule()
{
    AddModule<PlayerSessionModule>();
    AddModule<PlayerModule>();
    AddModule<PropModule>();
    AddModule<GameMasterModule>();
}

void GameServerModuleCenter::InitConfigModule()
{
    AddModule<PropConfigModule>();
}


