/*
 * CSLoginReqHelper.cpp
 *
 *  Created on: 2014年7月22日
 *  Author: jarek
 *  Brief:
 */

#include <Common/State/StateInterface.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Message/Message/CSRequest.h>
#include <Message/Message/CSResponse.h>
#include <Player/MessageHelper/CSLoginReqHelper.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/Msg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>
#include <Session/State/SessionStateDefine.h>
#include <string>


CSLoginReqHelper::CSLoginReqHelper()
{
    // NULL
}


CSLoginReqHelper::~CSLoginReqHelper()
{
    // NULL
}

void CSLoginReqHelper::OnGetCreateAsyncData(AsyncDataList& asyncDataList)
{
    // NULL
}

void CSLoginReqHelper::HandleMessage()
{
    CSRequest* request = dynamic_cast<CSRequest*>(message);
    ASSERT_RETURN_VOID(request != NULL && request->GetMessageID() == CS_MSG_LOGIN_REQ);

    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    const CSLoginReq& loginReq = request->GetDefaultMsg().body().cs_login_req();

    Player* oldPlayer = NULL;

    // 删除旧会话，如果存在的话
    PlayerSession* oldSession = sessionModule.GetSessionByAccount(loginReq.account().c_str());
    if (oldSession != NULL)
    {
        LOG_RUN("Session<%s> kickout by repeating login", oldSession->BriefInfo());
        oldPlayer = oldSession->GetPlayer();
        sessionModule.ReleaseSession(oldSession->GetSessionID());
    }

    // 创建新会话
    PlayerSession* newSession = sessionModule.CreateSession(loginReq.account().c_str());
    if (NULL == newSession)
    {
        LOG_ERR("Login fail, create session<account:%s> fail", loginReq.account().c_str());
        assert(false);

        CSResponse response(*request);
        response.AddErrorMsg(MSG_ERROR_UNDEFINE);
        response.Send();
    }

    // 如果旧player数据存在，新会话直接使用旧数据对象
    if (oldPlayer != NULL)
    {
        newSession->TakePlayer(oldPlayer);
    }

    LOG_RUN("Session<%s> login successfully", newSession->BriefInfo());

    newSession->GetStateInterface().ChangeCurState(SessionState::Online);

    request->GetMsgSet().mutable_head()->set_session_id(newSession->GetSessionID());
    request->GetMsgSet().mutable_head()->set_sequence(newSession->GetCSResponser().GetValidRequestSequence());
    SessionResponse(newSession, request);
}


void CSLoginReqHelper::HandleDataTimeOut()
{
    // NULL
}



