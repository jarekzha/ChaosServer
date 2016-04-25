/*
 * RobotPlayerManager.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */


#include <Engine/Object/New.h>
#include <Engine/Log/LogMacro.h>
#include <Framework/Robot.h>
#include <Framework/RobotConfig.h>
#include <Player/RobotPlayer.h>
#include <Player/RobotPlayerManager.h>
#include <stddef.h>

RobotPlayerManager::RobotPlayerManager()
{
    robotPlayerNum = 0;
    robotPlayer = NULL;
    stopRobotPlayerNum = 0;
}

RobotPlayerManager::~RobotPlayerManager()
{
    for (UINT i = 0; i < robotPlayerNum; ++i)
    {
        Delete(robotPlayer[i]);
    }

    DeleteArray(robotPlayer);
}

bool RobotPlayerManager::Initialize()
{
    robotPlayerNum = theRobot.GetConfig().RobotNum;
    ASSERT_RETURN(robotPlayerNum != 0, false);

    robotPlayer = NewArray<RobotPlayer*>(robotPlayerNum);
    for (UINT i = 0; i < robotPlayerNum; ++i)
    {
        robotPlayer[i] = NULL;
    }

    for (UINT i = 0; i < robotPlayerNum; ++i)
    {
        RobotPlayer* tempPlayer = New<RobotPlayer>();
        if (!tempPlayer->Initialize(i))
        {
            LOG_ERR("Initialize player fail");
            Delete(tempPlayer);

            return false;
        }

        robotPlayer[i] = tempPlayer;
    }

    return true;
}

void RobotPlayerManager::StartAction()
{
    for (UINT i = 0; i < robotPlayerNum; ++i)
    {
        robotPlayer[i]->StartAction();
    }
}

bool RobotPlayerManager::CheckAllStop() const
{
    return stopRobotPlayerNum == robotPlayerNum;
}

void RobotPlayerManager::AddStopRobotPlayerNum()
{
    ThreadMutexGuard lock(mutex);

    stopRobotPlayerNum++;
}
