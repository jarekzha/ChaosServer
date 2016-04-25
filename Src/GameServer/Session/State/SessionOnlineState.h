/*
 * PlayerOnlineState.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#pragma once

#include <Engine/State/State.h>
#include <Engine/Time/Timer.h>
#include <Session/State/SessionStateDefine.h>

class SessionOnlineState
    : public TState<SessionOnlineState, SessionState::Online>
{
public:
                        SessionOnlineState();

    // State
public:
    virtual void        OnEnter();
    virtual void        OnLeave();

private:
    void                HandleCSLoginReq(ProtoBuffMessage& msg);
    void                HandleCSNopRep(ProtoBuffMessage& msg);

private:
   void                 OnTimerActiveCheck();

private:
   Timer                activeCheckTimer;
};


