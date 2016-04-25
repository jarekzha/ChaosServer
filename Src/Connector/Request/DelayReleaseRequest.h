/*
 * DelayReleaseRequest.h
 *
 *  Created on: 2014年8月5日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Delay/DelayFunction.h>

class Request;

class DelayReleaseRequest : public DelayFunction
{
public:
    DelayReleaseRequest(Request* req);

    // DelayFunction
public:
    virtual void        Invoke();

private:
    int                 requestID;
};


