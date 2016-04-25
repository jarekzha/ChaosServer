/*
 * State.h
 *
 *  Created on: 2014年6月24日
 *      Author: jarek
 *  状态基类
 */

#pragma once

#include <Util/Util.h>
#include <Language/TypeID.h>
#include <Log/LogMacro.h>

class StateOwner;

/*
 * 状态接口
 */
class State
{
public:
    virtual             ~State(){}

    virtual void        OnEnter() = 0;

    virtual void        OnLeave() = 0;

    virtual const char* GetName() const = 0;

    virtual UINT        GetStateID() const = 0;

    virtual void        SetOwner(StateOwner* stateOwner) = 0;
};

/*
 * 状态模版实现，用于具体状态继承
 */
template <typename ConcreteState, UINT StateID>
class TState : public State
{
    // State
public:
    virtual void        OnEnter();

    virtual void        OnLeave();

    virtual const char* GetName() const;

    virtual UINT        GetStateID() const;

    virtual void        SetOwner(StateOwner* stateOwner);

public:
    // 状态ID
    const static UINT   ID = StateID;

protected:
    StateOwner*         stateOwner;     // 状态拥有者
};

template <typename ConcreteState, UINT StateID>
void TState<ConcreteState, StateID>::OnEnter()
{
    // NULL
}

template <typename ConcreteState, UINT StateID>
void TState<ConcreteState, StateID>::OnLeave()
{
    // NULL
}

template <typename ConcreteState, UINT StateID>
const char* TState<ConcreteState, StateID>::GetName() const
{
    return TypeID<ConcreteState>::Name.c_str();
}

template <typename ConcreteState, UINT StateID>
UINT TState<ConcreteState, StateID>::GetStateID() const
{
    return StateID;
}

template <typename ConcreteState, UINT StateID>
void TState<ConcreteState, StateID>::SetOwner(StateOwner* stateOwner)
{
    ASSERT_RETURN_VOID(stateOwner != NULL);
    this->stateOwner = stateOwner;
}


