/*
 * PlayerOnlineState.cpp
 *
 *  Created on: 2014年6月26日
 *      Author: jarek
 */

#include <Common/Message/ProtoBuffMessage.h>
#include <Common/State/StateInterface.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Time/TimerCallback.h>
#include <Engine/Util/Util.h>
#include <Message/Message/CSResponse.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Session/PlayerSession.h>
#include <Session/State/SessionOnlineState.h>


SessionOnlineState::SessionOnlineState()
{
    RegisterMsgFunc(CS_MSG_LOGIN_REQ, &SessionOnlineState::HandleCSLoginReq);
    RegisterMsgFunc(CS_MSG_NOP_REP, &SessionOnlineState::HandleCSNopRep);
}

void SessionOnlineState::OnEnter()
{
    activeCheckTimer.Start(TimerCallback(this, &SessionOnlineState::OnTimerActiveCheck), TIMER_MINUTE);
}

void SessionOnlineState::OnLeave()
{
    activeCheckTimer.Stop();
}

void SessionOnlineState::HandleCSLoginReq(ProtoBuffMessage& msg)
{
    PlayerSession* session = dynamic_cast<PlayerSession*>(stateOwner);
    ASSERT_RETURN_VOID(session != NULL);

    CSSubResponse response(*session);
    response.GetMsg().set_id(CS_MSG_LOGIN_RSP);
    response.Send();
}

void SessionOnlineState::HandleCSNopRep(ProtoBuffMessage& msg)
{
    PlayerSession* session = dynamic_cast<PlayerSession*>(stateOwner);
    ASSERT_RETURN_VOID(session != NULL);

    CSSubResponse response(*session);
    response.GetMsg().set_id(CS_MSG_NOP_REP);
    response.Send();
}

void SessionOnlineState::OnTimerActiveCheck()
{
    PlayerSession* session = dynamic_cast<PlayerSession*>(stateOwner);
    ASSERT_RETURN_VOID(session != NULL);

    if (!session->CheckActive())
    {
        LOG_RUN("Session<%s> is not active,change to offline", session->BriefInfo());
        session->GetStateInterface().ChangeCurState(SessionState::Offline);
    }
}

