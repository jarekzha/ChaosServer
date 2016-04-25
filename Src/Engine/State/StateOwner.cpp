/*
 * StateOwner.cpp
 *
 *  Created on: 2015年11月3日
 *  Author: jarek
 *  Brief: 
 */
#include <State/StateOwner.h>

StateOwner::StateOwner(StateInterface& stateInterface)
    : interface(stateInterface)
{
}

StateOwner::~StateOwner()
{
    // NULL
}

StateInterface& StateOwner::GetStateInterface()
{
    return interface;
}
