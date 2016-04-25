/*
 * ResponseThread.h
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Thread/Thread.h>
#include <Response/ResponseHandler.h>


class ResponseThread : public Thread
{
public:
    ResponseThread();
    virtual             ~ResponseThread();

protected:
    virtual void        Routine();

    virtual void        OnStop(){}

private:
    ResponseHandler     responseHandler;
};

