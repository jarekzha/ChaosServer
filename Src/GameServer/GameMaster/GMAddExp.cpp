/*
 * GMAddExp.cpp
 *
 *  Created on: 2014年10月20日
 *  Author: jarek
 *  Brief: 
 */
#include <GameMaster/GMAddExp.h>
#include <Engine/Log/LogMacro.h>
#include <Framework/GameServer.h>
#include <Player/PlayerModule.h>

bool GMAddExp::HandleCommand(std::vector<std::string>& httpCommandParamVec)
{
    //addexp name num
    if (httpCommandParamVec.size() < 3)
    {
        LOG_WARN("[GM-ADDEXP] param num[%u] invalid", (uint)httpCommandParamVec.size());
        return false;
    }

    std::string& playerName = httpCommandParamVec[1];
    int expAddVal = atoi(httpCommandParamVec[2].c_str());

    LOG_TRACE("[GM-ADDEXP] player[%s] expAddVal[%d]", playerName.c_str(), expAddVal);

    PlayerModule& playerModule = theModule(PlayerModule);
    Player* player = playerModule.GetPlayerByAccount(playerName);
    if (NULL == player)
    {
        LOG_WARN("[GM-ADDEXP] can't find player[%s]", playerName.c_str());
        return false;
    }

    player->GetAttribute().ChangeAttr(PLAYER_ATTR_TYPE_EXP, expAddVal);

    return true;
}

