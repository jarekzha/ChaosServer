/*
 * PlayerLoginState.cpp
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Message/Message/CSResponse.h>
#include <Message/Message/GameServerMessage.h>
#include <Message/MessageDefine.h>
#include <Player/GameState/PlayerLoadState.h>
#include <Player/Player.h>
#include <Player/PlayerModule.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <cstdio>
#include <string>

using namespace std;

PlayerLoadState::PlayerLoadState()
{
    RegisterMsgFunc(SS_MSG_QUERY_ROLE_RSP, &PlayerLoadState::HandleSSQueryRoleRsp);
    RegisterMsgFunc(SS_MSG_CREATE_ROLE_RSP, &PlayerLoadState::HandleSSCreateRoleRsp);
}

void PlayerLoadState::OnEnter()
{
    SendQueryRoleInfoReq();
}

void PlayerLoadState::OnLeave()
{
    // NULL
}

void PlayerLoadState::OnOffline()
{
    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    player->Release();
}

void PlayerLoadState::HandleSSQueryRoleRsp(ProtoBuffMessage& msg)
{
    const SSQueryRoleRsp& queryRoleRsp = msg.GetDefaultMsg().body().ss_query_role_rsp();

    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    if (!queryRoleRsp.query_successful())
    {
        if (queryRoleRsp.error_id() == MSG_ERROR_NO_DATA)
        {
            LOG_RUN("Player<%s> query role info no data when load", player->BriefInfo());
            CreateRole();
            return;
        }
        else
        {
            LOG_ERR("Player<%s> query role fail <errorID:%d>",
                    player->BriefInfo(), queryRoleRsp.error_id());

            CSSubResponse::SendErrorMsg(*player, MSG_ERROR_UNDEFINE);
            player->ReleaseGracefully();
            return;
        }
    }

    player->Load(&msg);
    return;
}

void PlayerLoadState::HandleSSCreateRoleRsp(ProtoBuffMessage& msg)
{
    const SSCreateRoleRsp& createRoleRsp = msg.GetDefaultMsg().body().ss_create_role_rsp();

    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    if (!createRoleRsp.successful())
    {
        LOG_ERR("Player<%s> create role fail <errorID:%d>",
                player->BriefInfo(), createRoleRsp.error_id());

        CSSubResponse::SendErrorMsg(*player, MSG_ERROR_UNDEFINE);
        player->ReleaseGracefully();
        return;
    }

    // 创建角色成功，重新发送角色信息查询请求
    LOG_RUN("Player<:%s> create role successfully requery to load", player->BriefInfo());
    SendQueryRoleInfoReq();
}

void PlayerLoadState::CreateRole()
{
    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    PlayerModule& playerModule = theModule(PlayerModule);
    Player* defaultPlayer = playerModule.GetDefaultPlayer(player->GetAccount());
    if (NULL == defaultPlayer)
    {
        assert(false);
        player->ReleaseGracefully();
        return;
    }

    LOG_RUN("Player<%s> send create role req", player->BriefInfo());

    GameServerMessage ssReqMsg(player->GetSessionID(), MessageChannelType::DBProxy);
    ssReqMsg.GetDefaultMsg().set_id(SS_MSG_CREATE_ROLE_REQ);
    SSCreateRoleReq* createRoleReq = ssReqMsg.GetDefaultMsg().mutable_body()->mutable_ss_create_role_req();
    createRoleReq->set_account(player->GetAccount());
    if (!defaultPlayer->StoreToDBBin(*createRoleReq->mutable_serialized_role_info()))
    {
        LOG_ERR("Player<%s> create role fail, store new player info to db blob fail",
                player->BriefInfo());
        player->ReleaseGracefully();
        return;
    }
    ssReqMsg.Send();
}

void PlayerLoadState::SendQueryRoleInfoReq()
{
    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    LOG_RUN("Player<%s> send query role req", player->BriefInfo());

    GameServerMessage request(*player, MessageChannelType::DBProxy);
    Msg& msg = request.GetDefaultMsg();
    msg.set_id(SS_MSG_QUERY_ROLE_REQ);
    msg.mutable_body()->mutable_ss_query_role_req()->set_account(player->GetAccount());
    request.Send();
}



