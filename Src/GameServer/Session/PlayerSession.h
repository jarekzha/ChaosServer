/*
 * PlayerSession.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Util/GameObject.h>
#include <Engine/State/StateInterface.h>
#include <Engine/Time/Timer.h>
#include <Session/CSResponser.h>
#include <string>

class Player;
class CSRequest;

class PlayerSession
        : public GameObject
        , public MessageHandler
{
public:
    PlayerSession();
    virtual             ~PlayerSession();

    // Visualizable
public:
    virtual const char* BriefInfo() const;

    // MessageHandler
public:
    virtual void        HandleMessage(ProtoBuffMessage& msg);

    // PlayerSession
public:
    const std::string&  GetAccount();
    void                SetAccount(const char* account);

    int                 GetSessionID();
    void                SetSessionID(int sessionID);

    StateInterface&     GetStateInterface();
    Player*             GetPlayer();
    CSResponser&        GetCSResponser();

    bool                CheckActive();
    void                ReleaseGracefully();
    void                Release();
    void                TakePlayer(Player* newPlayer);
    void                ClearPlayer();

private:
    void                OnReleaseGracefully();

private:
    int                 sessionID;          // 会话ID
    Player*             player;
    std::string         account;
    StateInterface      stateInterface;
    CSResponser         csResponser;
    Timer               gracefullyReleaseTimer;
};


