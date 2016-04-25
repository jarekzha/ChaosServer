/*
 * LogicThread.h
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/Thread.h>

class LogicThread : public Thread
{
public:
    virtual             ~LogicThread(){}

    // LogicThread
public:
    virtual void        Routine();

    virtual void        OnStop(){}
};


