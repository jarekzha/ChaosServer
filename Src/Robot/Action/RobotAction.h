/*
 * Action.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Language/Cloneable.h>
#include <Engine/Language/Visualizable.h>

class RobotPlayer;



class RobotAction
        : public Visualizable
        , public Cloneable<RobotAction>
{
public:
    virtual             ~RobotAction(){}

    // 执行
    virtual void        Execute(RobotPlayer& robotPlayer) = 0;
};















