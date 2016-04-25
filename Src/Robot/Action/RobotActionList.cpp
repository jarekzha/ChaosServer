/*
 * RobotActionList.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief:
 */

#include <Action/RobotActionList.h>
#include <Engine/Object/New.h>
#include <Engine/Util/Util.h>
#include <Player/RobotPlayer.h>
#include <Engine/Log/LogMacro.h>


RobotActionList::RobotActionList()
{
    // NULL
}

RobotActionList::~RobotActionList()
{
    Release();
}

std::string RobotActionList::BriefInfo() const
{
    return "name:RobotActionList";
}

RobotAction* RobotActionList::Clone() const
{
    RobotActionList* newRobotActionList = New<RobotActionList>();

    for (ActionList::const_iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
        RobotAction* robotAction = (*iter)->Clone();
        ASSERT_RETURN(robotAction != NULL, newRobotActionList);

        newRobotActionList->actionList.push_back(robotAction);
    }

    return newRobotActionList;
}

void RobotActionList::Execute(RobotPlayer& robotPlayer)
{
    UINT serialNumber = 0;
    for (ActionList::iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
        RobotAction* action = *iter;
        LOG_RUN("Player<%s> start execute action<%s, serialNum:%u>",
                robotPlayer.BriefInfo().c_str(), action->BriefInfo().c_str(), serialNumber);

        action->Execute(robotPlayer);

        LOG_RUN("Player<%s> end execute action<%s, serialNum:%u>",
                robotPlayer.BriefInfo().c_str(), action->BriefInfo().c_str(), serialNumber);

        serialNumber++;
    }
}

void RobotActionList::AddAction(RobotAction& robotAction)
{
    actionList.push_back(&robotAction);
}

void RobotActionList::Release()
{
    for (ActionList::iterator iter = actionList.begin(); iter != actionList.end(); ++iter)
    {
        Delete(*iter);
    }

    actionList.clear();
}

