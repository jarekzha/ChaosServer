/*
 * PlayerLogoutState.cpp
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/ShmObject/IDPtr.h>
#include <Engine/Util/Util.h>
#include <Message/Message/CSResponse.h>
#include <Message/Message/GameServerMessage.h>
#include <Message/MessageDefine.h>
#include <Player/GameState/PlayerReleaseState.h>
#include <Player/Player.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <string>

using namespace std;

PlayerReleaseState::PlayerReleaseState()
{
    RegisterMsgFunc(SS_MSG_UPDATE_ROLE_RSP, &PlayerReleaseState::HandleUpdateRoleRsp);
}

void PlayerReleaseState::OnEnter()
{
    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    player->SendUpdateRoleMsg();

    timeout.Start(TimerCallback(this, &PlayerReleaseState::OnTimeout), 5 * TIMER_SECOND);
}

void PlayerReleaseState::OnTimeout()
{
    timeout.Stop();

    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    player->Release();
}

void PlayerReleaseState::HandleUpdateRoleRsp(ProtoBuffMessage& msg)
{
    Player* player = dynamic_cast<Player*>(stateOwner);
    ASSERT_RETURN_VOID(player != NULL);

    const SSUpdateRoleRsp& updateRoleRsp = msg.GetDefaultMsg().body().ss_update_role_rsp();

    if (!updateRoleRsp.update_successful())
    {
        LOG_ERR("Player<%s> release fail, update role info fail",
                player->BriefInfo());

        CSSubResponse::SendErrorMsg(*player, MSG_ERROR_UNDEFINE);
        return;
    }

    // 资源回收
    player->ReleaseGracefully();
}
