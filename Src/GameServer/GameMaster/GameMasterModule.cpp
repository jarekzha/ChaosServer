/*
 * GMUtil.cpp
 *
 *  Created on: 2014年9月1日
 *      Author: silas
 */

#include <GameMaster/GameMasterModule.h>
#include <Engine/Object/New.h>
#include <Engine/Log/LogMacro.h>
#include <GameMaster/GMAddExp.h>
#include <GameMaster/GMAddItem.h>
#include <stdlib.h>

GameMasterModule::GameMasterModule()
    :gmCommandHandlerMap()
{
    // NULL
}

GameMasterModule::~GameMasterModule()
{
    for (GMCommandHanlderMap::iterator iter = gmCommandHandlerMap.begin();
            iter != gmCommandHandlerMap.end(); ++iter)
    {
        Delete(iter->second);
    }

    gmCommandHandlerMap.clear();
}

bool GameMasterModule::Initialize()
{
    gmCommandHandlerMap["additem"] =New<GMAddItem>();
    gmCommandHandlerMap["addexp"] = New<GMAddExp>();

    return true;
}

bool GameMasterModule::HandleGMRequest(std::vector<std::string>& httpCommandParamVec)
{
    if (httpCommandParamVec.empty() || httpCommandParamVec.size() <= 2)
    {
        return false;
    }

    std::string& command = httpCommandParamVec[0];
    GMCommandHanlderMap::iterator iter = gmCommandHandlerMap.find(command);
    if (iter != gmCommandHandlerMap.end())
    {
        GMCommand* commandHandler = iter->second;
        commandHandler->HandleCommand(httpCommandParamVec);
    }
    else
    {
        LOG_WARN("[GMUTIL] command[%s] can't find gmhandler", command.c_str());
    }

    return true;
}
