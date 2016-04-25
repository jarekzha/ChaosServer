/*
 * GMConnectorMessageChannel.h
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#pragma once
#include <Common/Message/IPCMessageChannel.h>

class GMConnectorMessageChannel : public IPCMessageChannel
{
public:
    GMConnectorMessageChannel();
    virtual             ~GMConnectorMessageChannel();

    // IPCMessageChannel
private:
    virtual bool        InitConfig(Config& config);
};

