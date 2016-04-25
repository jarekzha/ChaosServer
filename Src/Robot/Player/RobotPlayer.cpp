/*
 * RobotPlayer.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief:
 */

#include <Action/RobotActionTemplate.h>
#include <boost/bind/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>
#include <Engine/Object/New.h>
#include <Framework/Robot.h>
#include <Player/RobotPlayer.h>
#include <Player/RobotPlayerManager.h>
#include <cstdio>

using namespace std;

RobotPlayer::RobotPlayer()
{
    sessionID = 0;
    sequence = 0;
    action = NULL;

    keyInit = false;
    defaultKey[0] = teaKey[0] = 0x12345678;
    defaultKey[1] = teaKey[1] = 0x734a67fc;
    defaultKey[2] = teaKey[2] = 0x3367a642;
    defaultKey[3] = teaKey[3] = 0x78432562;
}

RobotPlayer::~RobotPlayer()
{
    Delete(action);
}

std::string RobotPlayer::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "account:" << account << ", sessionID:" << sessionID;
    return briefInfo.str();
}

bool RobotPlayer::Initialize(UINT index)
{
    account = "Robot_" + boost::lexical_cast<string>(index);

    action = theRobot.GetActionListTemplate().GetActionList().Clone();
    ASSERT_RETURN(action != NULL, false);

    return true;
}

void RobotPlayer::SetSessionID(int sessionID)
{
    this->sessionID = sessionID;
}

int RobotPlayer::GetSessionID()
{
    return sessionID;
}

const std::string& RobotPlayer::GetAccount()
{
    return account;
}

void RobotPlayer::SetSequence(int sequence)
{
    this->sequence = sequence;
}

int RobotPlayer::GetSequence()
{
    return sequence;
}

void RobotPlayer::StartAction()
{
    boost::function0<void> f = boost::bind(&RobotPlayer::ExecuteAction, this);
    thread = boost::shared_ptr<boost::thread>(New<boost::thread>(f));
}

void RobotPlayer::ExecuteAction()
{
    action->Execute(*this);
    theRobot.GetPlayerManager().AddStopRobotPlayerNum();
}

bool RobotPlayer::IsKeyInit() const
{
    return keyInit;
}

void RobotPlayer::SetTeaKey(const XXTEA_TYPE* teaKey)
{
    if (NULL == teaKey)
    {
        return ;
    }

    keyInit = true;
    this->teaKey[0] = teaKey[0];
    this->teaKey[1] = teaKey[1];
    this->teaKey[2] = teaKey[2];
    this->teaKey[3] = teaKey[3];
}

const XXTEA_TYPE* RobotPlayer::GetTeaKey()
{
    return teaKey;
}

const XXTEA_TYPE* RobotPlayer::GetDefaultKey()
{
    return defaultKey;
}
