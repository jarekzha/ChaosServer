/*
 * RobotActionListTemplate.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Action/RobotAction.h>
#include <list>

class RobotActionList : public RobotAction
{
public:
    RobotActionList();
    virtual             ~RobotActionList();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // Cloneable<RobotAction>
public:
    virtual RobotAction* Clone() const;

    // RobotAction
public:
    virtual void        Execute(RobotPlayer& robotPlayer);

    // RobotActionList
public:
    void                AddAction(RobotAction& robotAction);

private:
    void                Release();

private:
    typedef std::list<RobotAction*> ActionList;

private:
    ActionList          actionList;
};




