/*
 * StateInterface.cpp
 *
 *  Created on: 2014年6月24日
 *      Author: jarek
 */

#include <Log/LogMacro.h>
#include <Factory/Factory.h>
#include <Object/New.h>
#include <State/StateInterface.h>
#include <State/StateOwner.h>

StateInterface::StateInterface(StateOwner* stateOwner,  Factory<State>* stateFactory)
{
    ASSERT_RETURN_VOID(stateOwner != NULL);
    this->stateOwner = stateOwner;
    this->stateFactory = stateFactory;
    curState = NULL;
}

StateInterface::~StateInterface()
{
    Delete(curState);
}

void StateInterface::ChangeCurState(int newStateID)
{
    int oldStateID = -1;
    if (curState != NULL)
    {
        oldStateID = curState->GetStateID();
    }

    if (oldStateID == newStateID)
    {
        return;
    }

    // 离开并删除旧状态
    if (curState != NULL)
    {
        curState->OnLeave();
        Delete(curState);
    }

    // 创建新状态
    curState = stateFactory->Create(newStateID);
    if (NULL == curState)
    {
        LOG_ERR("Change state <oldID:%u> to <newID:%u> fail, invalid new ID",
                oldStateID, newStateID);
        return;
    }

    // 进入新状态
    curState->SetOwner(stateOwner);
    curState->OnEnter();

    LOG_TRACE("%s change state <oldID:%d> to <newID:%d>",
            VariableTypeName(*stateOwner).c_str(), oldStateID, newStateID);
}

int StateInterface::GetCurStateID() const
{
    if (NULL == curState)
    {
        return 0;
    }

    return curState->GetStateID();
}

State* StateInterface::GetCurState() const
{
    return curState;
}

