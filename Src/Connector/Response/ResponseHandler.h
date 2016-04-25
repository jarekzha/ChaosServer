/*
 * ResponseHandler.h
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Protocol/NetMsg.pb.h>


class ResponseHandler
{
public:
    ResponseHandler();
    virtual             ~ResponseHandler();

public:
    void                HandleResponse();

private:
    bool                RecvFromGameServer();

    void                DoResponse();

private:
    MsgSet              responseMsgSet;
};


