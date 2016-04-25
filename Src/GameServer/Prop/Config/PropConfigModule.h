/*
 * PropConfigManager.h
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Framework/Module.h>


class PropConfigModule : public ConfigModule
{
public:
    PropConfigModule();
    virtual             ~PropConfigModule();

    // Module
public:
    virtual bool        Initialize();

private:
    void                AddItemConfig();
};



