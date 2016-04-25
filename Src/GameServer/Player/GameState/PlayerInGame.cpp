/*
 * PlayerStateWorldMap.cpp
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#include <Common/State/StateInterface.h>
#include <Dungeon/DungeonProgress.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Equipment/EquipmentOperationHandler.h>
#include <Fragment/FragmentOperationHandler.h>
#include <Framework/GameServer.h>
#include <Hero/HeroOperationHandler.h>
#include <Mall/MallModule.h>
#include <Mall/SellItemHandler.h>
#include <Message/Message/CSResponse.h>
#include <Player/GameState/PlayerInGame.h>
#include <Player/Player.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Skill/SkillOperationHandler.h>
#include <Lottery/LotteryHandler.h>
#include <Mail/MailOperationHandler.h>
#include <AdviserSkill/AdviserSkillOperationHandler.h>
#include <Engine/Framework/ModuleCenter.h>
#include <Guide/GuideModule.h>
#include <cstdio>


PlayerInGame::PlayerInGame()
    : player(NULL)
{
    RegisterMsgFunc(CS_MSG_LOGOUT_REQ, &PlayerInGame::HandleCSLogoutReq);
    RegisterMsgFunc(CS_MSG_GET_PLAYER_DATA_REQ, &PlayerInGame::HandleCSGetPlayerDataReq);
    RegisterMsgFunc(CS_MSG_START_CHALLENGE_LEVEL_REQ, &PlayerInGame::HandleCSStartChallengeLevelReq);
    RegisterMsgFunc(CS_MSG_FINISH_CHALLENGE_LEVEL_REQ, &PlayerInGame::HandleCSFinishChallengeLevelReq);
    RegisterMsgFunc(CS_MSG_HERO_COMPOSE_REQ, &PlayerInGame::HandleCSHeroComposeReq);
    RegisterMsgFunc(CS_MSG_HERO_UPGRADE_REQ, &PlayerInGame::HandleCSHeroLvUpReq);
    RegisterMsgFunc(CS_MSG_HERO_EVOLUTION_REQ, &PlayerInGame::HandleCSHeroEvolutionReq);
    RegisterMsgFunc(CS_MSG_HERO_WEAR_EQUIPMENT_REQ, &PlayerInGame::HandleCSHeroWearEquipReq);
    RegisterMsgFunc(CS_MSG_SELL_ITEM_REQ, &PlayerInGame::HandleCSSellItemReq);
    RegisterMsgFunc(CS_MSG_HERO_SKILL_UPGRADE_REQ, &PlayerInGame::HandleCSHeroSkillUpgrade);
    RegisterMsgFunc(CS_MSG_FRAGMENT_COMPOSE_REQ, &PlayerInGame::HandleCSFragmentComposeReq);
    RegisterMsgFunc(CS_MSG_EQUIPMENT_COMPOSE_REQ, &PlayerInGame::HandleCSEquipmentComposeReq);
    RegisterMsgFunc(CS_MSG_BUY_ITEM_REQ, &PlayerInGame::HandleCSBuyItemReq);
    RegisterMsgFunc(CS_MSG_ASK_UPDATE_MALL_REQ, &PlayerInGame::HandleCSAskUpdMallReq);
    RegisterMsgFunc(CS_MSG_LOTTERY_REQ, &PlayerInGame::HandleCSLotteryReq);
    RegisterMsgFunc(CS_MSG_ASK_LOTTERY_INFO_REQ, &PlayerInGame::HandleCSAskLotteryReq);
    RegisterMsgFunc(CS_MSG_MAIL_LIST_REQ, &PlayerInGame::HandleCSListMailReq);
    RegisterMsgFunc(CS_MSG_MAIL_OPERATION_REQ, &PlayerInGame::HandleCSMailOpReq);
    RegisterMsgFunc(CS_MSG_ADVISER_SKILL_COMPOSE_REQ, &PlayerInGame::HandleCSAdviserSkillComposeReq);
    RegisterMsgFunc(CS_MSG_ADVISER_SKILL_EVOLUTION_REQ, &PlayerInGame::HandleCSAdviserSkillEvolutionReq);
    RegisterMsgFunc(CS_MSG_EXCHANGE_REQ, &PlayerInGame::HandleCSExchangeReq);
    RegisterMsgFunc(CS_MSG_GUIDE_DROP_REQ, &PlayerInGame::HandleCSGuideDropReq);
}

void PlayerInGame::OnEnter()
{
    player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    activeCheckTimer.Start(TimerCallback(this, &PlayerInGame::OnTimerActiveCheck), TIMER_MINUTE);
    updateRoleTimer.Start(TimerCallback(this, &PlayerInGame::OnTimerUpdateRole), TIMER_MINUTE);
}

void PlayerInGame::OnLeave()
{
    activeCheckTimer.Stop();
    updateRoleTimer.Stop();

    player = NULL;
}

void PlayerInGame::OnOffline()
{
    player->GetStateInterface().ChangeCurState(PlayerState::Release);
}

void PlayerInGame::OnTimerActiveCheck()
{
    if (player->CheckActive())
    {
        return;
    }

    LOG_RUN("Player<%s> is not active, change to release", player->BriefInfo());
    player->GetStateInterface().ChangeCurState(PlayerState::Release);
}

void PlayerInGame::OnTimerUpdateRole()
{
    player->SendUpdateRoleMsg();
}

void PlayerInGame::HandleCSLogoutReq(ProtoBuffMessage& msg)
{
    OnOffline();
}

void PlayerInGame::HandleCSGetPlayerDataReq(ProtoBuffMessage& msg)
{
    CSSubResponse response(*player);
    Msg& responseMsg = response.GetMsg();

    responseMsg.set_id(CS_MSG_GET_PLAYER_DATA_RSP);
    if (!player->FillCSInfo(*responseMsg.mutable_body()->mutable_cs_get_player_data_rsp()))
    {
        LOG_ERR("Player<%s> fill cs info fail", player->BriefInfo());
        CSSubResponse::SendErrorMsg(*player, MSG_ERROR_INVALID_DATA);
        return;
    }

    response.Send();
}

void PlayerInGame::HandleCSStartChallengeLevelReq(ProtoBuffMessage& msg)
{
    player->GetDungeonProgress().StartChallengeLevel(msg.GetDefaultMsg().body().cs_start_challenge_level_req());
}

void PlayerInGame::HandleCSFinishChallengeLevelReq(ProtoBuffMessage& msg)
{
    player->GetDungeonProgress().FinishChallengeLevel(msg.GetDefaultMsg().body().cs_finish_challenge_level_req());
}

void PlayerInGame::HandleCSHeroComposeReq(ProtoBuffMessage& msg)
{
    HeroOperationHandler::Instance().HeroCompose(*player, msg.GetDefaultMsg().body().cs_hero_compose_req());
}

void PlayerInGame::HandleCSHeroLvUpReq(ProtoBuffMessage& msg)
{
    HeroOperationHandler::Instance().HeroUpgrade(player, msg.GetDefaultMsg().body().cs_hero_upgrade_req());
}

void PlayerInGame::HandleCSHeroEvolutionReq(ProtoBuffMessage& msg)
{
    HeroOperationHandler::Instance().HeroEvolution(*player, msg.GetDefaultMsg().body().cs_hero_evolution_req());
}

void PlayerInGame::HandleCSHeroWearEquipReq(ProtoBuffMessage& msg)
{
    HeroOperationHandler::Instance().HeroWearEquip(player, msg.GetDefaultMsg().body().cs_hero_wearequip_req());
}

void PlayerInGame::HandleCSSellItemReq(ProtoBuffMessage& msg)
{
    theModule(MallModule).GetSellItemHandler().Sell(*player,
            msg.GetDefaultMsg().body().cs_sell_item_req());
}

void PlayerInGame::HandleCSHeroSkillUpgrade(ProtoBuffMessage& msg)
{
    SkillOperationHandler::Instance().SkillUpgrade(player, msg.GetDefaultMsg().body().cs_hero_upgrade_skill_req());
}

void PlayerInGame::HandleCSFragmentComposeReq(ProtoBuffMessage& msg)
{
    FragmentOperationHandler::Instance().FragmentCompose(*player, msg.GetDefaultMsg().body().cs_fragment_compose_req());
}

void PlayerInGame::HandleCSEquipmentComposeReq(ProtoBuffMessage& msg)
{
    EquipmentOperationHandler::Instance().EquipmentCompose(*player, msg.GetDefaultMsg().body().cs_equipment_compose_req());
}

void PlayerInGame::HandleCSBuyItemReq(ProtoBuffMessage& msg)
{
    theModule(MallModule).GetBuyItemHandler().Buy(*player,
            msg.GetDefaultMsg().body().cs_buy_item_req());
}

void PlayerInGame::HandleCSAskUpdMallReq(ProtoBuffMessage& msg)
{
    theModule(MallModule).GetBuyItemHandler().AskUpdateMall(*player,
            msg.GetDefaultMsg().body().cs_ask_update_mall_req());
}

void PlayerInGame::HandleCSLotteryReq(ProtoBuffMessage& msg)
{
    LotteryHandler::Instance().HandleLottery(*player, msg.GetDefaultMsg().body().cs_lottery_req());
}

void PlayerInGame::HandleCSAskLotteryReq(ProtoBuffMessage& msg)
{
    LotteryHandler::Instance().HandleAskLottery(*player);
}

void PlayerInGame::HandleCSListMailReq(ProtoBuffMessage& msg)
{
    MailOperationHandler::Instance().ListMail(*player);
}

void PlayerInGame::HandleCSMailOpReq(ProtoBuffMessage& msg)
{
    MailOperationHandler::Instance().EmailOperation(*player, msg.GetDefaultMsg().body().cs_mail_operation_req());
}

void PlayerInGame::HandleCSAdviserSkillComposeReq(ProtoBuffMessage& msg)
{
    AdviserSkillOperationHandler::Instance().Compose(*player, msg.GetDefaultMsg().body().cs_adviser_skill_compose_req());
}

void PlayerInGame::HandleCSAdviserSkillEvolutionReq(ProtoBuffMessage& msg)
{
    AdviserSkillOperationHandler::Instance().Evolution(*player, msg.GetDefaultMsg().body().cs_adviser_skill_evolution_req());
}

void PlayerInGame::HandleCSExchangeReq(ProtoBuffMessage& msg)
{
    theModule(MallModule).GetBuyItemHandler().Exchange(*player,
            msg.GetDefaultMsg().body().cs_exchange_req());
}

void PlayerInGame::HandleCSGuideDropReq(ProtoBuffMessage& msg)
{
    theModule(GuideModule).HandleGuideDrop(*player, msg.GetDefaultMsg().body().cs_guide_drop_req());
}
