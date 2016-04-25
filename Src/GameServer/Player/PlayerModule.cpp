#include <Engine/Object/New.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Player/PlayerModule.h>
#include <map>

using namespace std;

PlayerModule::PlayerModule()
{
}

PlayerModule::~PlayerModule()
{
    // 先取出player，放在一个list里统一删除，因为删除player的过程中，player会反注册，从而修改accountPlayerMap
    list<Player*> playerList;
    for (AccountPlayerMap::iterator iter = accountPlayerMap.begin();
            iter != accountPlayerMap.end(); ++iter)
    {
        playerList.push_back(iter->second);
    }

    for (list<Player*>::iterator iter = playerList.begin();
            iter != playerList.end(); ++iter)
    {
        Delete(*iter);
    }
}

bool PlayerModule::Initialize()
{
    return true;
}

Player* PlayerModule::GetOrCreatePlayer(const string& account)
{
    Player* player = GetPlayerByAccount(account);
    if (player != NULL)
    {
        return player;
    }

    player = CreatePlayer(account);
    return player;
}

Player* PlayerModule::CreatePlayer(const string& account)
{
    Player* player = New<Player>();

    ASSERT_RETURN(player != NULL, NULL);

    player->Initialize();
    player->SetAccount(account);

    RegisterPlayer(player);

    return player;
}

void PlayerModule::ReleasePlayer(Player* player)
{
    // 反注册
    UnregisterPlayer(player);

    // 释放对象
    Delete(player);
}

void PlayerModule::UnregisterPlayer(Player* player)
{
    LOG_TRACE("Unregister player <sessionID:%d> <accountID:%s> <name:%s>",
            player->GetSessionID(), player->GetAccount().c_str(),
            player->GetAttribute().GetName().c_str());

    if (player->GetAccount() != "")
    {
        accountPlayerMap.erase(player->GetAccount());
    }

    if (player->GetAttribute().GetName() != "")
    {
        playerNamePlayerMap.erase(player->GetAttribute().GetName());
    }

    LOG_RUN("After unregister PlayerModule: <accountMap:%lu>, <rolenameMap:%lu>",
            accountPlayerMap.size(), playerNamePlayerMap.size());
}

Player* PlayerModule::GetPlayerByAccount(const string& account)
{
    AccountPlayerMap::iterator iter = accountPlayerMap.find(account);
    if (iter != accountPlayerMap.end())
    {
        return iter->second;
    }

    return NULL;
}

Player* PlayerModule::GetPlayerByPlayerName(const string& name)
{
    PlayerNamePlayerMap::iterator iter = playerNamePlayerMap.find(name);
    if (iter != playerNamePlayerMap.end())
    {
        return iter->second;
    }

    return NULL;
}

Player* PlayerModule::GetDefaultPlayer(const string& account)
{
    static Player* defaultPlayer = NULL;
    if (defaultPlayer != NULL)
    {
        defaultPlayer->SetAccount(account);
        defaultPlayer->GetAttribute().SetName(account);
        return defaultPlayer;
    }

    defaultPlayer = New<Player>();
    defaultPlayer->Initialize();
    defaultPlayer->SetAccount(account);
    defaultPlayer->GetAttribute().SetName(account);
    defaultPlayer->InitNewRole();

    return defaultPlayer;
}

void PlayerModule::RegisterPlayer(Player* player)
{
    TRACE("Register player <sessionID:%d> <accountID:%s> <name:%s>",
            player->GetSessionID(), player->GetAccount().c_str(),
            player->GetAttribute().GetName().c_str());

    if (player->GetAccount() != "")
    {
        accountPlayerMap[player->GetAccount()] = player;
    }

    if (player->GetAttribute().GetName() != "")
    {
        playerNamePlayerMap[player->GetAttribute().GetName()] = player;
    }

    LOG_RUN("After register PlayerModule: <accountMap:%lu>, <rolenameMap:%lu>",
            accountPlayerMap.size(), playerNamePlayerMap.size());
}

