/*
 * Prop.h
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/BagItem.h>
#include <Engine/Util/Util.h>
#include <Prop/PropDefine.h>

class PropConfigItem;

class Prop : public BagItem
{
public:
    Prop();
    virtual             ~Prop();

    // BagItemExtendable
public:
    virtual UINT        GetBagItemResID() const;

    virtual bool        Initialize(UINT resID);

    // Prop
public:
    PropType::Enum      GetPropType() const;

    // Cloneable<BagItem>
public:
    virtual BagItem*    Clone() const;

private:
    UINT                resID;
    shared_ptr<const PropConfigItem> config;
};




