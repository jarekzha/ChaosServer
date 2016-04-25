 /*
 * Player.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */
#pragma once

#include <Engine/AsyncData/AsyncData.h>
#include <Engine/Message/Handler/MessageHandler.h>
#include <Engine/State/StateInterface.h>
#include <Engine/Time/Timer.h>
#include <Player/PlayerAttribute.h>
#include <Player/PlayerBagMap.h>
#include <Util/GameObject.h>
#include <string>

class ProtoBuffMessage;

class PlayerSession;

/*
 * player玩家，代表每一个登录上线玩家实体
 */
class Player 
    : public GameObject
    , public MessageHandler
    , public AsyncData
{
public:
                        Player();
    virtual             ~Player();

    // MessageHandler
public:
    virtual void        HandleMessage(ProtoBuffMessage& msg);

    // AsyncData
public:
    virtual void        Release();
    virtual void        OnStartLoad();
    virtual bool        OnLoad(AsyncResponse* response);

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // Player
public:
    bool                Initialize();

    // 初始化新角色
    void                InitNewRole();

    bool                CheckActive() const;
    void                ReleaseGracefully();
    void                Release();

    void                SetSessionID(int sessionID);
    int                 GetSessionID();
    PlayerSession*      GetSession() const;
    
    void                SetAccount(const std::string& account);
    const std::string&  GetAccount();

    StateInterface&     GetStateInterface();
    PlayerAttribute&    GetAttribute();
    PlayerBagMap&       GetBagMap();

    bool                FillCSInfo(CSGetPlayerDataRsp& playerInfo);
    bool                RestoreFromDBBin(const std::string& binStr);
    bool                StoreToDBBin(std::string& binStr);
    void                SendUpdateRoleMsg();

private:
    void                OnReleaseGracefully();

private:
    StateInterface      stateInterface;
    PlayerAttribute     attribute;
    PlayerBagMap        bagMap;

    int                 sessionID;
    std::string         account;
    Timer               gracefullyReleaseTimer;
};





