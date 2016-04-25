/*
 * PlayerSessionModule.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Framework/Module.h>
#include <Session/PlayerSession.h>
#include <map>
#include <string>

class PlayerSession;

class PlayerSessionModule : public LogicModule
{
public:
    PlayerSessionModule();
    virtual             ~PlayerSessionModule();

    // LogicModule
public:
    virtual bool        Initialize();

    virtual void        StartGracefullyStop();

    virtual bool        CheckGracefullyStopFinished();

    // PlayerSessionModule
public:
    PlayerSession*      CreateSession(const char* account);

    void                ReleaseSession(int sessionID);

    PlayerSession*      GetSessionBySessionID(int sessionID);

    PlayerSession*      GetSessionByAccount(const char* account);

private:
    void                RegisterSession(PlayerSession* session);

    void                UnregisterSession(PlayerSession* session);

    bool                AssertSessionIsUnregister(PlayerSession* session);

    int                 AllocateNewSessionID();

private:
    typedef std::map<int, PlayerSession*>           SessionIDSessionMap;
    typedef std::map<std::string, PlayerSession*>   AccountSessionMap;

    int                     maxSessionID;
    SessionIDSessionMap     sessionIDSessionMap;
    AccountSessionMap       accountSessionMap;
};



