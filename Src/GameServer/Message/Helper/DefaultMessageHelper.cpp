/*
 * DefaultMessageHelper.cpp
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Message/Message/GameServerMessage.h>
#include <Player/Player.h>
#include <Protocol/Macro.pb.h>
#include <Session/PlayerSession.h>
#include <Session/PlayerSessionModule.h>
#include <Src/GameServer/Message/Helper/DefaultMessageHandler.h>


DefaultMessageHelper::DefaultMessageHelper()
{
    // NULL
}

DefaultMessageHelper::~DefaultMessageHelper()
{
    // NULL
}

void DefaultMessageHelper::OnGetCreateAsyncData(AsyncDataList& asyncDataList)
{
    // NULL
}

void DefaultMessageHelper::HandleMessage()
{
    GameServerMessage* gamesvrMsg = dynamic_cast<GameServerMessage*>(message);
    ASSERT_RETURN_VOID(gamesvrMsg != NULL);

    PlayerSessionModule& sessionModule = theModule(PlayerSessionModule);
    PlayerSession* session = sessionModule.GetSessionBySessionID(gamesvrMsg->GetSessionID());
    if (NULL == session)
    {
        return;
    }

    // player处理消息
    session->HandleMessage(*gamesvrMsg);

    session->GetCSResponser().TryFinishResponse(session);
}

void DefaultMessageHelper::HandleDataTimeOut()
{
    // NULL
}

