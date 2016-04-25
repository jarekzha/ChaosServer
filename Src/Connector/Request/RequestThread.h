/*
 * RequestThread.h
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/Thread.h>

class RequestThread : public Thread
{
public:
    RequestThread();
    virtual             ~RequestThread();

protected:
    virtual void        Routine();

    virtual void        OnStop(){}
};

