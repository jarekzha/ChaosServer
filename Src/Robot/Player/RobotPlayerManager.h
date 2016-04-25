/*
 * RobotPlayerManager.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/ThreadMutex.h>
#include <Engine/Util/Util.h>

class RobotPlayer;

class RobotPlayerManager
{
public:
    RobotPlayerManager();
    virtual             ~RobotPlayerManager();

public:
    bool                Initialize();

    void                StartAction();

    bool                CheckAllStop() const;

    void                AddStopRobotPlayerNum();

private:
    UINT                robotPlayerNum;
    UINT                stopRobotPlayerNum; // 已停止acitonlist的机器人player
    RobotPlayer**       robotPlayer;
    ThreadMutex         mutex;
};





