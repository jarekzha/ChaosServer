/*
 * DBProxyMessageChannel.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/IPCMessageChannel.h>

class DBProxyMessageChannel : public IPCMessageChannel
{
public:
    DBProxyMessageChannel();
    virtual             ~DBProxyMessageChannel();

    // IPCMessageChannel
private:
    virtual bool        InitConfig(Config& config);
};


