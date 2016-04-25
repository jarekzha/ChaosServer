#pragma once

#include <Engine/Framework/Module.h>
#include <Player/Player.h>
#include <map>
#include <string>

/*
 * player模块
 */
class PlayerModule : public LogicModule
{
public:
                        PlayerModule();
    virtual             ~PlayerModule();

    // LogicModule
public:
    virtual bool        Initialize();

    // PlayerModule
public:
    Player*             GetOrCreatePlayer(const std::string& account);

    Player*             CreatePlayer(const std::string& account);

    void                ReleasePlayer(Player* player);

    Player*             GetPlayerByAccount(const std::string& account);

    Player*             GetPlayerByPlayerName(const std::string& name);

    Player*             GetDefaultPlayer(const std::string& account);

private:
    void                RegisterPlayer(Player* player);

    void                UnregisterPlayer(Player* player);

private:
    typedef std::map<std::string, Player*>  AccountPlayerMap;
    typedef std::map<std::string, Player*>  PlayerNamePlayerMap;

    AccountPlayerMap    accountPlayerMap;
    PlayerNamePlayerMap playerNamePlayerMap;
};

