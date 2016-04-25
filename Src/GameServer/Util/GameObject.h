/*
 * GameObject.h
 *
 *  Created on: 2015年11月15日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Language/Visualizable.h>
#include <Engine/Object/UniqueObject.h>

class GameObject : public UniqueObject<GameObject>, public Visualizable
{
    // NULL
};


