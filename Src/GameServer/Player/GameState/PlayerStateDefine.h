/*
 * PlayerStateDefine.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Factory/Factory.h>
#include <Engine/State/State.h>
#include <Engine/Util/TMTSingleton.h>

struct PlayerState
{
    typedef enum
    {
        Load                = 1,  // 玩家数据加载中
        Release             = 2,  // 玩家释放
        InGame              = 3,  // 玩家在游戏中

    } Enum;
};


class PlayerStateFactory
        : public Factory<State>
        , public TMTSingleton<PlayerStateFactory>
{
public:
    PlayerStateFactory();
};
