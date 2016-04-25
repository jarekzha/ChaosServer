/*
 * PlayerSessionManager.cpp
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Object/New.h>
#include <Engine/Object/ShmNew.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>

PlayerSessionModule::PlayerSessionModule()
{
    maxSessionID = 0;
}

PlayerSessionModule::~PlayerSessionModule()
{
    for (SessionIDSessionMap::iterator iter = sessionIDSessionMap.begin();
            iter != sessionIDSessionMap.end(); iter++)
    {
        Delete(iter->second);
    }

    sessionIDSessionMap.clear();
    accountSessionMap.clear();
}

bool PlayerSessionModule::Initialize()
{
    maxSessionID = 0;
    return true;
}

void PlayerSessionModule::StartGracefullyStop()
{
    for (SessionIDSessionMap::iterator iter = sessionIDSessionMap.begin();
            iter != sessionIDSessionMap.end(); ++iter)
    {
        PlayerSession* session = iter->second;
        session->ReleaseGracefully();
    }
}

bool PlayerSessionModule::CheckGracefullyStopFinished()
{
    return sessionIDSessionMap.empty() && accountSessionMap.empty();
}

PlayerSession* PlayerSessionModule::CreateSession(const char* account)
{
    PlayerSession* session = New<PlayerSession>();

    session->SetSessionID(AllocateNewSessionID());
    session->SetAccount(account);

    LOG_RUN("Create session<%s>", session->BriefInfo());

    RegisterSession(session);
    return session;
}

void PlayerSessionModule::ReleaseSession(int sessionID)
{
    PlayerSession* session = GetSessionBySessionID(sessionID);
    if (NULL == session)
    {
        LOG_ERR("Release session<ID:%d> fail, not found", sessionID);
        return;
    }

    UnregisterSession(session);

    LOG_RUN("Release session<%s>", session->BriefInfo());
    Delete(session);
}

void PlayerSessionModule::RegisterSession(PlayerSession* session)
{
    ASSERT_RETURN_VOID(session != NULL);

    if (!AssertSessionIsUnregister(session))
    {
        LOG_ERR("Register sessioin<%s> fail", session->BriefInfo());
        return;
    }

    if (session->GetSessionID() != 0)
    {
        sessionIDSessionMap[session->GetSessionID()] = session;
    }

    if (session->GetAccount() != "")
    {
        accountSessionMap[session->GetAccount()] = session;
    }

    LOG_RUN("Register session<%s>", session->BriefInfo());
}

void PlayerSessionModule::UnregisterSession(PlayerSession* session)
{
    if (session->GetSessionID() != 0)
    {
        sessionIDSessionMap.erase(session->GetSessionID());
    }

    if (session->GetAccount() != "")
    {
        accountSessionMap.erase(session->GetAccount());
    }

    LOG_RUN("Unregister session<%s>", session->BriefInfo());
}

int PlayerSessionModule::AllocateNewSessionID()
{
    maxSessionID++;
    if (0 == maxSessionID)
    {
        maxSessionID++;
    }

    return maxSessionID;
}

PlayerSession* PlayerSessionModule::GetSessionBySessionID(int sessionID)
{
    SessionIDSessionMap::iterator iter = sessionIDSessionMap.find(sessionID);
    if (iter == sessionIDSessionMap.end())
    {
        return NULL;
    }

    return iter->second;
}

PlayerSession* PlayerSessionModule::GetSessionByAccount(const char* account)
{
    AccountSessionMap::iterator iter = accountSessionMap.find(account);
    if (iter == accountSessionMap.end())
    {
        return NULL;
    }

    return iter->second;
}

bool PlayerSessionModule::AssertSessionIsUnregister(PlayerSession* session)
{
    if (session->GetSessionID() != 0)
    {
        if (sessionIDSessionMap.find(session->GetSessionID()) != sessionIDSessionMap.end())
        {
            LOG_ERR("Assert session unregister fail, session<%s> is register in sessionIDMap",
                                session->BriefInfo());
            return false;
        }
    }

    if (session->GetAccount() != "")
    {
        if (accountSessionMap.find(session->GetAccount()) != accountSessionMap.end())
        {
            LOG_ERR("Assert session unregister fail, session<%s> is register in accountMap",
                    session->BriefInfo());
            return false;
        }
    }

    return true;
}















