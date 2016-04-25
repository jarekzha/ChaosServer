/*
 * StateInterface.h
 *
 *  Created on: 2014年6月24日
 *      Author: jarek
 *  管理状态，改变状态，提供接口，间接操作State接口
 */

#pragma once

#include <Util/Util.h>
#include <State/State.h>
#include <Factory/Factory.h>

class StateOwner;

/**
 * 状态接口
 */
class StateInterface
{
public:
                        StateInterface(StateOwner* stateOwner, Factory<State>* stateFactory);
    virtual             ~StateInterface();

    // StateInterface
public:
    // 改变当前状态
    void                ChangeCurState(int newStateID);

    // 获取当前状态的ID
    int                 GetCurStateID() const;

    // 获取当前状态
    State*              GetCurState() const;

private:
    StateOwner*         stateOwner;
    State*              curState;
    Factory<State>*     stateFactory;
};


