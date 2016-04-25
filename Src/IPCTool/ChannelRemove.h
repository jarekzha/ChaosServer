/*
 * QueueRemover.h
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <string>
#include <Executable.h>

class ChannelRemove : public Executable
{
public:
    ChannelRemove();
    virtual             ~ChannelRemove();

    // Executable
public:
    virtual void        Execute(const IPCConfigFile& config);

private:
    void                RemoveChannel(const std::string& channelName);
};



