/*
 * PlayerLogoutState.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/State/State.h>
#include <Engine/Time/Timer.h>
#include <Player/GameState/PlayerStateDefine.h>

class PlayerReleaseState
        : public TState<PlayerReleaseState, PlayerState::Release>
{
public:
                        PlayerReleaseState();

// State
public:
    virtual void        OnEnter();

private:
    void                OnTimeout();

private:
    void                HandleUpdateRoleRsp(ProtoBuffMessage& msg);

private:
    Timer               timeout;
};


