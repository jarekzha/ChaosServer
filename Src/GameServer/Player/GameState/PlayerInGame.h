/*
 * PlayerStateWorldMap.h
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 *  角色玩家视角在世界地图的状态
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/State/State.h>
#include <Engine/Time/Timer.h>
#include <Player/GameState/PlayerStateDefine.h>
#include <Player/Util/PlayerOfflineHandler.h>

class Player;

/**
 * 角色在游戏中状态
 */
class PlayerInGame
        : public TState<PlayerInGame, PlayerState::InGame>
        , public PlayerOfflineHandler
{
public:
                        PlayerInGame();

// State
public:
    virtual void        OnEnter();
    virtual void        OnLeave();

// PlayerOfflineHandler
protected:
    virtual void        OnOffline();

private:
    void                HandleCSLogoutReq(ProtoBuffMessage& msg);
    void                HandleCSGetPlayerDataReq(ProtoBuffMessage& msg);
    void                HandleCSStartChallengeLevelReq(ProtoBuffMessage& msg);
    void                HandleCSFinishChallengeLevelReq(ProtoBuffMessage& msg);
    void                HandleCSHeroComposeReq(ProtoBuffMessage& msg);
    void                HandleCSHeroLvUpReq(ProtoBuffMessage& msg);
    void                HandleCSHeroEvolutionReq(ProtoBuffMessage& msg);
    void                HandleCSHeroWearEquipReq(ProtoBuffMessage& msg);
    void                HandleCSSellItemReq(ProtoBuffMessage& msg);
    void                HandleCSHeroSkillUpgrade(ProtoBuffMessage& msg);
    void                HandleCSFragmentComposeReq(ProtoBuffMessage& msg);
    void                HandleCSEquipmentComposeReq(ProtoBuffMessage& msg);
    void                HandleCSBuyItemReq(ProtoBuffMessage& msg);
    void                HandleCSAskUpdMallReq(ProtoBuffMessage& msg);
    void                HandleCSLotteryReq(ProtoBuffMessage& msg);
    void                HandleCSAskLotteryReq(ProtoBuffMessage& msg);
    void                HandleCSListMailReq(ProtoBuffMessage& msg);
    void                HandleCSMailOpReq(ProtoBuffMessage& msg);
    void                HandleCSAdviserSkillComposeReq(ProtoBuffMessage& msg);
    void                HandleCSAdviserSkillEvolutionReq(ProtoBuffMessage& msg);
    void                HandleCSExchangeReq(ProtoBuffMessage& msg);
    void                HandleCSGuideDropReq(ProtoBuffMessage& msg);

private:
    void                OnTimerActiveCheck();
    void                OnTimerUpdateRole();

private:
    Player*             player;
    Timer               activeCheckTimer;
    Timer               updateRoleTimer;
};


