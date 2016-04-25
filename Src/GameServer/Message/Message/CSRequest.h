/*
 * CSRequestMessage.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>

class CSRequest : public ProtoBuffMessage
{
public:
    CSRequest();
    virtual             ~CSRequest();

    // Message
public:
    virtual bool        IsValid();

    // CSRequest
public:
    int                 GetRequestID() const;
};


