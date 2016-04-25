/*
 * PlayerSession.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Time/Time.h>
#include <Engine/Time/TimerCallback.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Player/Player.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>
#include <Session/State/SessionStateDefine.h>
#include <cstdio>

using namespace std;


PlayerSession::PlayerSession()
        : stateInterface(this, &SessionStateFactory::Instance())
{
    sessionID = 0;
    player = NULL;
}

PlayerSession::~PlayerSession()
{
    // NULL
}

const char* PlayerSession::BriefInfo() const
{
    static char briefInfo[BRIEF_INFO_SIZE] = {};

    briefInfo[0] = 0;
    snprintf(briefInfo, sizeof(briefInfo), "account:%s, sessionID:%d",
            account.c_str(), sessionID);

    return briefInfo;
}

void PlayerSession::HandleMessage(ProtoBuffMessage& msg)
{
    stateInterface.HandleMessage(msg);

    if (player != NULL)
    {
        player->GetStateInterface().HandleMessage(msg);
    }
}

const string& PlayerSession::GetAccount()
{
    return account;
}

void PlayerSession::SetAccount(const char* account)
{
    this->account = account;
}

int PlayerSession::GetSessionID()
{
    return sessionID;
}

void PlayerSession::SetSessionID(int sessionID)
{
    this->sessionID = sessionID;
}

StateInterface& PlayerSession::GetStateInterface()
{
    return stateInterface;
}

Player* PlayerSession::GetPlayer()
{
    return player;
}

CSResponser& PlayerSession::GetCSResponser()
{
    return csResponser;
}

bool PlayerSession::CheckActive()
{
    ULONG curSecond = Time::GetCurSecond();
    return (curSecond - csResponser.GetLastRequestSec()) < 36000;
}

void PlayerSession::ReleaseGracefully()
{
    if (player != NULL)
    {
        player->ReleaseGracefully();
    }

    gracefullyReleaseTimer.Start(TimerCallback(this, &PlayerSession::OnReleaseGracefully), TIMER_SECOND);
}

void PlayerSession::Release()
{
    theModule(PlayerSessionModule).ReleaseSession(GetSessionID());
}

void PlayerSession::TakePlayer(Player* newPlayer)
{
    ASSERT_RETURN_VOID(newPlayer != NULL);

    if (player != NULL)
    {
        if (newPlayer->GetObjectID() == player->GetObjectID())
        {
            return;
        }
    }

    player = newPlayer;
    player->SetSessionID(GetSessionID());
}

void PlayerSession::ClearPlayer()
{
    player = NULL;
}

void PlayerSession::OnReleaseGracefully()
{
    if (player != NULL)
    {
        // 等待player退出
        return;
    }
    else
    {
        gracefullyReleaseTimer.Stop();
        Release();
    }
}








