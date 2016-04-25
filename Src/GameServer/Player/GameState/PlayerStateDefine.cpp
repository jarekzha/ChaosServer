/*
 * PlayerStateDefine.cpp
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief:
 */

#include <Player/GameState/PlayerInGame.h>
#include <Player/GameState/PlayerLoadState.h>
#include <Player/GameState/PlayerReleaseState.h>
#include <Player/GameState/PlayerStateDefine.h>

PlayerStateFactory::PlayerStateFactory()
{
    RegisterProduct<PlayerLoadState>(PlayerState::Load);
    RegisterProduct<PlayerInGame>(PlayerState::InGame);
    RegisterProduct<PlayerReleaseState>(PlayerState::Release);
}



