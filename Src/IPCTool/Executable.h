/*
 * Executable.h
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Engine/IPC/IPCConfig.h>

class Executable
{
public:
    virtual             ~Executable(){}

    virtual void        Execute(const IPCConfigFile& config) = 0;
};


