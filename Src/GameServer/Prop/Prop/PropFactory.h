/*
 * PropFactory.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Factory/Factory.h>
#include <Engine/Util/TMTSingleton.h>
#include <Prop/Prop/AddExpProp.h>
#include <Prop/PropDefine.h>

/**
 * 道具工厂
 */
class PropFactory
        : public Factory<Prop>
        , public TMTSingleton<PropFactory>
{
public:
    PropFactory()
    {
        RegisterProduct<Prop>(PropType::Common);
        RegisterProduct<AddExpProp>(PropType::AddExp);
    }
};



