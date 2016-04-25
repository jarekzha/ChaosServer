/*
 * MessageThread.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/Thread.h>

class MessageThread : public Thread
{
public:
    MessageThread();
    virtual             ~MessageThread();

    // MessageThread
protected:
    virtual void        Routine();

    virtual void        OnStop(){}
};


