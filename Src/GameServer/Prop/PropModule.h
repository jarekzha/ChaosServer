/*
 * PropModule.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Framework/Module.h>

class PropModule
        : public LogicModule
        , public BagItemCreator
{
public:
    PropModule();
    virtual             ~PropModule();

    // Module
public:
    virtual bool        Initialize();

    // BagItemCreator
public:
    virtual BagItem*    CreateBagItem(UINT resID);

private:
};


