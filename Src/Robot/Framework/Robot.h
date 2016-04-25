/*
 * Robot.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Action/RobotActionTemplate.h>
#include <Engine/Util/TMTSingleton.h>
#include <Framework/RobotConfig.h>
#include <Player/RobotPlayerManager.h>
#include <string>

class Robot : public TMTSingleton<Robot>
{
public:
    Robot();
    virtual             ~Robot();

public:
    bool                Initialize(const std::string& configFilePath);

    void                Run();

public:
    const RobotConfigItem& GetConfig();

    RobotPlayerManager& GetPlayerManager();

    RobotActionTemplate& GetActionListTemplate();

private:
    bool                continueFlag;
    RobotConfigFile     configFile;
    RobotPlayerManager  playerManager;
    RobotActionTemplate actionListTemplate;
};

#define theRobot    Robot::Instance()






