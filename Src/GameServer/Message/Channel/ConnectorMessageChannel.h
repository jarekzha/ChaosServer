/*
 * ConnectorMessageChannel.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/IPCMessageChannel.h>


class ConnectorMessageChannel : public IPCMessageChannel
{
public:
    ConnectorMessageChannel();
    virtual             ~ConnectorMessageChannel();

    // IPCMessageChannel
private:
    virtual bool        InitConfig(Config& config);
};

