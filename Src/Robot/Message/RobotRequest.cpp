/*
 * RobotRequest.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#include <Message/RobotRequest.h>
#include <Player/RobotPlayer.h>
#include <Protocol/NetMsg.pb.h>



RobotRequest::RobotRequest(RobotPlayer& robotPlayer)
    : player(robotPlayer)
{
    protoBuffMsgSet.mutable_head()->set_session_id(robotPlayer.GetSessionID());
    protoBuffMsgSet.mutable_head()->set_sequence(robotPlayer.GetSequence() + 1);
}

RobotPlayer& RobotRequest::GetRobotPlayer() const
{
    return player;
}
