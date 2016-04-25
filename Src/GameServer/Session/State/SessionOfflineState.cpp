/*
 * PlayerOfflineState.cpp
 *
 *  Created on: 2014年6月25日
 *      Author: jarek
 */

#include <Common/State/StateInterface.h>
#include <Engine/ShmObject/IDPtr.h>
#include <Engine/Time/Timer.h>
#include <Engine/Time/TimerCallback.h>
#include <Engine/Util/Util.h>
#include <Player/Player.h>
#include <Player/Util/PlayerOfflineHandler.h>
#include <Session/PlayerSession.h>
#include <Session/State/SessionOfflineState.h>

SessionOfflineState::SessionOfflineState()
{
    // NULL
}

SessionOfflineState::~SessionOfflineState()
{
    // NULL
}

void SessionOfflineState::OnEnter()
{
    PlayerSession* session = dynamic_cast<PlayerSession*>(stateOwner);
    ASSERT_RETURN_VOID(session != NULL);

    session->ReleaseGracefully();
}

void SessionOfflineState::OnLeave()
{

}
