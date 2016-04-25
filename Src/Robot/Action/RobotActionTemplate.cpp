/*
 * RobotActionTemplate.cpp
 *
 *  Created on: 2014年7月30日
 *  Author: jarek
 *  Brief: 
 */

#include <Action/RobotActionTemplate.h>
#include <Engine/Object/New.h>
#include <Protocol/Macro.pb.h>

bool RobotActionTemplate::Initialize()
{
  //  actionList.AddAction(*New<RobotLoginAction>());

    return true;
}

const RobotActionList& RobotActionTemplate::GetActionList()
{
    return actionList;
}
