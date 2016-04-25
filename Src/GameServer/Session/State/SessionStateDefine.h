/*
 * SessionStateDefine.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Factory/Factory.h>
#include <Engine/State/State.h>
#include <Engine/Util/TMTSingleton.h>

struct SessionState
{
    typedef enum
    {
        Online      = 1,
        Offline     = 2,
    } Enum;
};


class SessionStateFactory
        : public Factory<State>
        , public TMTSingleton<SessionStateFactory>
{
public:
    SessionStateFactory();
};
