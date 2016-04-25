/*
 * ChannelShow.h
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Executable.h>
#include <string>

class ChannelShow : public Executable
{
public:
    ChannelShow();
    virtual             ~ChannelShow();

public:
    virtual void        Execute(const IPCConfigFile& config);

private:
    void                ShowChannel(std::string& name);
};


