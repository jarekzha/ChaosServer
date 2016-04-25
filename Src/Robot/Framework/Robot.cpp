/*
 * Robot.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Config/ConfigFile.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Time/Time.h>
#include <Framework/Robot.h>
#include <Message/HttpClient.h>

Robot::Robot()
{
    continueFlag = true;
}

Robot::~Robot()
{

}

bool Robot::Initialize(const std::string& configFilePath)
{
    if (!configFile.LoadFromXml(configFilePath.c_str()))
    {
        LOG_ERR("Load xml config file<path:%s> fail", configFilePath.c_str());
        return false;
    }

    const RobotConfigItem& configItem = configFile.Item();
    HttpClient::Instance().Initialize(configItem.ServerHost, configItem.ServerPort, false);

    if (!actionListTemplate.Initialize())
    {
        LOG_ERR("RobotActionTemplate initialize fail");
        return false;
    }

    if (!playerManager.Initialize())
    {
        LOG_ERR("RobotPlayerManager initialize fail");
        return false;
    }

    LOG_RUN("Robot initialize successfully");
    return true;
}

void Robot::Run()
{
    LOG_RUN("Robot run");
    playerManager.StartAction();

    int updateCount = 0;
    while (true)
    {
        Time::UpdateTime();
        Time::MsSleep(100);

        updateCount++;
        if (updateCount > 10)
        {
            updateCount = 0;
            if (playerManager.CheckAllStop())
            {
                break;
            }
        }
    }

    LOG_RUN("All robot player stop");
}

const RobotConfigItem& Robot::GetConfig()
{
    return configFile.Item();
}

RobotPlayerManager& Robot::GetPlayerManager()
{
    return playerManager;
}

RobotActionTemplate& Robot::GetActionListTemplate()
{
    return actionListTemplate;
}








