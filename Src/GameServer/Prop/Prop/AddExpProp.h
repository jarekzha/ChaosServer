/*
 * AddExpProp.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Prop/Prop/Prop.h>

class Player;

/**
 * 添加经验道具
 */
class AddExpProp : public Prop
{
public:
    AddExpProp();
    virtual             ~AddExpProp();

    // Clone<BagItem>
public:
    virtual BagItem*    Clone() const;

    // AddExpProp
public:
    void                Use(Player& target);
};


