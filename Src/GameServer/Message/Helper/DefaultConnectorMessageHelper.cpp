/*
 * DefalutMessageHelper.cpp
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/AsyncData/AsyncData.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Message/Message/CSRequest.h>
#include <Message/Message/CSResponse.h>
#include <Player/Player.h>
#include <Player/PlayerModule.h>
#include <Protocol/Macro.pb.h>
#include <Session/CSResponser.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>
#include <Common/Util/Macro.h>
#include <Engine/Framework/ModuleCenter.h>
#include <Src/GameServer/Message/Helper/DefaultConnectorMessageHandler.h>

DefaultConnectorMessageHelper::DefaultConnectorMessageHelper()
{
    // NULL
}

DefaultConnectorMessageHelper::~DefaultConnectorMessageHelper()
{
    // NULL
}

void DefaultConnectorMessageHelper::OnGetCreateAsyncData(AsyncDataList& asyncDataList)
{
    CSRequest* request = dynamic_cast<CSRequest*>(message);
    ASSERT_RETURN_VOID(request != NULL);

    PlayerModule& playerModule = theModule(PlayerModule);
    PlayerSessionModule& sessionModule = theGameServer.GetModuleCenter().Get<PlayerSessionModule>();

    PlayerSession* session = sessionModule.GetSessionBySessionID(request->GetSessionID());
    if (NULL == session)
    {
        LOG_WARN("CSRequest<%s> deal async data fail, no session found", request->BriefInfo());
        return;
    }

    Player* player = playerModule.GetOrCreatePlayer(session->GetAccount());
    session->TakePlayer(player);

    if (player->GetDataState() != ASYNC_DATA_COMPLETE)
    {
        asyncDataList.push_back(player);
    }
}

void DefaultConnectorMessageHelper::HandleMessage()
{
    CSRequest* request = dynamic_cast<CSRequest*>(message);
    ASSERT_RETURN_VOID(request != NULL);

    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    PlayerSession* session = sessionModule.GetSessionBySessionID(request->GetSessionID());
    if (NULL == session)
    {
        LOG_WARN("CSRequest<%s> handle fail, no session found", request->BriefInfo());
        return;
    }

    SessionResponse(session, request);
}

void DefaultConnectorMessageHelper::HandleDataTimeOut()
{
    CSRequest* request = dynamic_cast<CSRequest*>(message);
    ASSERT_RETURN_VOID(request != NULL);

    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    PlayerSession* session = sessionModule.GetSessionBySessionID(request->GetSessionID());
    ASSERT_RETURN_VOID(session != NULL);

    LOG_ERR("Request<%s> session<%s> wait data time out", request->DetailInfo(), session->BriefInfo());

    CSResponse response(*request);
    response.AddErrorMsg(MSG_ERROR_WAIT_DATA_TIMEOUT);
    response.Send();
}

void DefaultConnectorMessageHelper::SessionResponse(PlayerSession* session, CSRequest* request)
{
    CSResponser& csResponser = session->GetCSResponser();

    // 标记开始处理请求
    csResponser.StartResponse(request);

    // 处理消息
#if MSG_TRACE_ON
    LOG_TRACE("Session<%s> handle request<%s>", session->BriefInfo(), request->BriefInfo());
#endif

    session->HandleMessage(*request);

    // 结束应答
    csResponser.TryFinishResponse(session);
}

