/*
 * PlayerLoginState.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 *  玩家角色登录状态
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/State/State.h>
#include <Player/GameState/PlayerStateDefine.h>
#include <Player/Util/PlayerOfflineHandler.h>

class Player;

class PlayerLoadState
        : public TState<PlayerLoadState, PlayerState::Load>
        , public PlayerOfflineHandler
{
public:
                        PlayerLoadState();

// State
public:
    virtual void        OnEnter();
    virtual void        OnLeave();

// PlayerOfflineHandler
protected:
    virtual void        OnOffline();

private:
    void                HandleSSQueryRoleRsp(ProtoBuffMessage& msg);
    void                HandleSSCreateRoleRsp(ProtoBuffMessage& msg);

private:
    void                CreateRole();
    void                SendQueryRoleInfoReq();
};

