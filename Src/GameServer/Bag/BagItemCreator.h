/*
 * BagItemCreator.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/Util.h>

class BagItem;

class BagItemCreator
{
public:
    virtual             ~BagItemCreator(){}

    virtual BagItem*    CreateBagItem(UINT resID) = 0;
};


