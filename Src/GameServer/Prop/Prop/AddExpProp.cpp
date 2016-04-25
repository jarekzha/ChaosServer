/*
 * AddExpProp.cpp
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Prop/Prop/AddExpProp.h>


AddExpProp::AddExpProp()
{
    // NULL
}

AddExpProp::~AddExpProp()
{
    // NULL
}

BagItem* AddExpProp::Clone() const
{
    return New<AddExpProp>(*this);
}

void AddExpProp::Use(Player& target)
{
    // JAREKTODO
}

