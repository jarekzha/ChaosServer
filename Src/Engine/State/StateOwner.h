/*
 * StateOwner.h
 *
 *  Created on: 2015年11月3日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

class StateInterface;

/**
 * 状态拥有者
 */
class StateOwner
{
public:
    StateOwner(StateInterface& stateInterface);
    virtual             ~StateOwner();

public:
    StateInterface&     GetStateInterface();

private:
    StateInterface&     interface;
};


