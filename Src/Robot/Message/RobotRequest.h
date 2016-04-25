/*
 * RobotRequest.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Common/Message/ProtoBuffMessage.h>

class RobotPlayer;


class RobotRequest : public ProtoBuffMessage
{
public:
    RobotRequest(RobotPlayer& robotPlayer);

public:
    RobotPlayer&        GetRobotPlayer() const;

private:
    RobotPlayer&        player;
};



