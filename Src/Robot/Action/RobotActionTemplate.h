/*
 * RobotActionTemplate.h
 *
 *  Created on: 2014年7月30日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Action/RobotActionList.h>

class RobotActionTemplate
{
public:
    bool                Initialize();

    const RobotActionList& GetActionList();

private:
    RobotActionList     actionList;
};


