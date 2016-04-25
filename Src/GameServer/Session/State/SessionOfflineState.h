/*
 * PlayerOfflineState.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#pragma once

#include <Engine/State/State.h>
#include <Session/State/SessionStateDefine.h>

class SessionOfflineState
        : public TState<SessionOfflineState, SessionState::Offline>
{
public:
                        SessionOfflineState();
    virtual             ~SessionOfflineState();

    // State
public:
    virtual void        OnEnter();
    virtual void        OnLeave();

private:

};



