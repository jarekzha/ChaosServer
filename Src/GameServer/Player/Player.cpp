/*
 * Player.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Common/State/State.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/State/State.h>
#include <Engine/Time/Time.h>
#include <Engine/Time/TimerCallback.h>
#include <Engine/Util/Assert.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServerConfig.h>
#include <google/protobuf/repeated_field.h>
#include <Message/Message/GameServerMessage.h>
#include <Message/MessageDefine.h>
#include <Player/GameState/PlayerStateDefine.h>
#include <Player/Player.h>
#include <Player/Util/PlayerOfflineHandler.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Session/CSResponser.h>
#include <Session/PlayerSession.h>
#include <cassert>
#include <sstream>

class PlayerModule;
class PlayerSessionModule;

using namespace std;


Player::Player()
    : attribute(*this)
    , stateInterface(this, &PlayerStateFactory::Instance())
    , bagMap(*this)
{
    sessionID = 0;

    SetGetDataIntervalMS(5000);
}

Player::~Player()
{
    PlayerSession* session = GetSession();
    if (session != NULL)
    {
        session->ClearPlayer();
    }
}

void Player::HandleMessage(ProtoBuffMessage& msg)
{
    stateInterface.HandleMessage(msg);
}

void Player::Release()
{
    ReleaseGracefully();
}

void Player::OnStartLoad()
{
    stateInterface.ChangeCurState(PlayerState::Load);
}

bool Player::OnStartLoad(AsyncResponse* response)
{
    ASSERT_RETURN(response != NULL, false);

    ProtoBuffMessage& rspMsg = dynamic_cast<ProtoBuffMessage&>(*response);
    Msg& loadMsg = rspMsg.GetDefaultMsg();
    ASSERT_RETURN(loadMsg.id() == SS_MSG_QUERY_ROLE_RSP, false);
    const SSQueryRoleRsp& queryRoleRsp = loadMsg.body().ss_query_role_rsp();

    if (!RestoreFromDBBin(queryRoleRsp.serialized_role_info()))
    {
        LOG_ERR("Player<%s> load fail", BriefInfo());
        assert(false);
        return false;
    }

    LOG_RUN("Player<:%s> load successfully", BriefInfo());

    stateInterface.ChangeCurState(PlayerState::InGame);
    return true;
}

string Player::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo <<  "account:" << account << ", sessionID:" << sessionID;

    return briefInfo.str();
}

bool Player::Initialize()
{
    return true;
}

void Player::InitNewRole()
{
}

bool Player::CheckActive() const
{
    const GameServerConfigItem& config = GameServerConfigFile::Instance().Item();

    PlayerSession* session = GetSession();
    ASSERT_RETURN(session != NULL, false);

    CSResponser& responser = session->GetCSResponser();
    ULONG curSecond = Time::GetCurSecond();
    ULONG lastRequestSecond = responser.GetLastRequestSec();

    if (curSecond > lastRequestSecond && curSecond - lastRequestSecond > config.PlayerActiveSecond)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Player::Release()
{
    LOG_RUN("Player<%s> release", BriefInfo());
    theModule(PlayerModule).ReleasePlayer(this);
}

void Player::ReleaseGracefully()
{
    LOG_RUN("Player<%s> release gracefully", BriefInfo());
    if (gracefullyReleaseTimer.Going())
    {
        LOG_TRACE("Player<%s> release gracefully is going", BriefInfo());
        return;
    }
    gracefullyReleaseTimer.Start(TimerCallback(this, &Player::OnReleaseGracefully), TIMER_SECOND);
}

void Player::SetSessionID(int sessionID)
{
    this->sessionID = sessionID;
}

int Player::GetSessionID()
{
    return this->sessionID;
}

PlayerSession* Player::GetSession() const
{
    if (0 == sessionID)
    {
        return NULL;
    }

    return theModule(PlayerSessionModule).GetSessionBySessionID(sessionID);
}

void Player::SetAccount(const string& account)
{
    this->account = account;
}

const std::string& Player::GetAccount()
{
    return this->account;
}

StateInterface& Player::GetStateInterface()
{
    return stateInterface;
}

PlayerAttribute& Player::GetAttribute()
{
    return attribute;
}

PlayerBagMap& Player::GetBagMap()
{
    return bagMap;
}

DungeonProgress& Player::GetDungeonProgress()
{
    return dungeonProgress;
}

MallCenter& Player::GetMallCenter()
{
    return mallCenter;
}

LotteryData& Player::GetLotteryData()
{
    return lotteryData;
}

MailBox& Player::GetMailBox()
{
    return mailBox;
}

GuideRecord& Player::GetGuideRecord()
{
    return guideRecord;
}

bool Player::FillCSInfo(CSGetPlayerDataRsp& playerInfo)
{
    bool res = true;
    playerInfo.set_account(account);
    res = res && attribute.FillCSInfo(*playerInfo.mutable_base());
    res = res && bagMap.FillCSInfo(playerInfo);
    res = res && dungeonProgress.FillCSInfo(*playerInfo.mutable_dungeon());

    return res;
}

bool Player::RestoreFromDBBin(const std::string& binStr)
{
    SSPlayerDBInfo dbInfo;
    if (!dbInfo.ParseFromString(binStr))
    {
        LOG_ERR("Player<%s> RestoreFromDBBin fail, parse bin fail", BriefInfo());
        assert(false);
        return false;
    }

    bool res = true;
    res = res && attribute.RestoreFromDB(dbInfo.base());
    res = res && bagMap.RestoreFromDB(dbInfo);
    res = res && dungeonProgress.RestoreFromDB(dbInfo.dungeon_progress());
    res = res && mallCenter.RestoreFromDB(dbInfo.mall());
    res = res && lotteryData.RestoreFromDBBin(dbInfo.lottery());
    res = res && guideRecord.RestoreFromDB(dbInfo.guide());

    return res;
}

bool Player::StoreToDBBin(std::string& binStr)
{
    bool res = true;

    SSPlayerDBInfo dbInfo;
    res = res && attribute.StoreToDB(*dbInfo.mutable_base());
    res = res && bagMap.StoreToDB(dbInfo);
    res = res && dungeonProgress.StoreToDB(*dbInfo.mutable_dungeon_progress());
    res = res && mallCenter.StoreToDB(*dbInfo.mutable_mall());
    res = res && lotteryData.FillCSInfo(*dbInfo.mutable_lottery());
    res = res && guideRecord.StoreToDB(*dbInfo.mutable_guide());

    if (!res)
    {
        LOG_ERR("Player<%s> StoreToDBBin fill msg fail", BriefInfo());
        assert(false);
        return false;
    }

    if (!dbInfo.SerializeToString(&binStr))
    {
        LOG_ERR("Player<%s> StoreToDBBin fail, parse bin fail", BriefInfo());
        assert(false);
        return false;
    }

    return true;
}

void Player::SendUpdateRoleMsg()
{
    LOG_RUN("Player<%s> send update role msg", BriefInfo());

    GameServerMessage gsMsg(sessionID, MessageChannelType::DBProxy);
    gsMsg.GetDefaultMsg().set_id(SS_MSG_UPDATE_ROLE_REQ);
    SSUpdateRoleReq& updateRoleReq = *gsMsg.GetDefaultMsg().mutable_body()->mutable_ss_update_role_req();

    updateRoleReq.set_account(account);
    if (!StoreToDBBin(*updateRoleReq.mutable_serialized_role_info()))
    {
        LOG_ERR("Player<%s> store to db bin fail",
                BriefInfo());
        return;
    }

    if (!mailBox.StoreMailToDBBin(*updateRoleReq.mutable_serialized_mail_info()))
    {
        LOG_ERR("Player<%s> store mail to db bin fail",
                BriefInfo());
        return;
    }

    gsMsg.Send();
}

void Player::OnReleaseGracefully()
{
    State* curPlayerState = stateInterface.GetCurState();
    if (curPlayerState != NULL)
    {
        PlayerOfflineHandler* offlineHandler = dynamic_cast<PlayerOfflineHandler*>(curPlayerState);
        if (offlineHandler != NULL)
        {
            offlineHandler->HandleOffline();
        }
    }
}
