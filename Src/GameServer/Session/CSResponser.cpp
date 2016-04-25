/*

 * Responser.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Time/Time.h>
#include <Engine/Util/FakeRandom.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Message/Message/CSRequest.h>
#include <Message/Message/CSResponse.h>
#include <Session/CSResponser.h>
#include <Session/PlayerSession.h>


CSResponser::CSResponser()
{
    Init();
}

CSResponser::~CSResponser()
{
}

void CSResponser::Init()
{
    validRequestSequence = (int)FakeRandom::Instance().Random();
    lastRequestSec = Time::GetCurSecond();
    curRequest = NULL;
    responseMsgList.clear();
}

bool CSResponser::StartResponse(CSRequest* request)
{
    ASSERT_RETURN(request != NULL, false);
    ASSERT_RETURN(CheckRequestSequence(request), false);

    curRequest = request;
    lastRequestSec = Time::GetCurSecond();

    return true;
}

void CSResponser::AddSubResponse(const NetMsg& msg)
{
    responseMsgList.push_back(msg);
}

CSRequest* CSResponser::GetCurRequest()
{
    return curRequest;
}

void CSResponser::TryFinishResponse(PlayerSession* session)
{
    if (!HaveRequest())
    {
        return;
    }

    if (responseMsgList.empty())
    {
        return;
    }

    CSResponse response(*curRequest);

    response.GetMsgSet().mutable_head()->set_session_id(session->GetSessionID());
    response.GetMsgSet().mutable_head()->set_sequence(validRequestSequence + 1);

    for (ResponseMsgList::iterator iter = responseMsgList.begin();
            iter != responseMsgList.end(); ++iter)
    {
        response.AddMsg(*iter);
    }

    response.Send();

    // 各种重置，等待下一个包
    validRequestSequence += 2;
    curRequest = NULL;
    responseMsgList.clear();
}

void CSResponser::AbortResponse()
{
    curRequest = NULL;
    responseMsgList.clear();
}

bool CSResponser::CheckRequestSequence(CSRequest* request)
{
    return request->GetSequenceID() == validRequestSequence;
}

ULONG CSResponser::GetLastRequestSec()
{
    return lastRequestSec;
}

int CSResponser::GetValidRequestSequence()
{
    return validRequestSequence;
}

bool CSResponser::HaveRequest()
{
    return curRequest != NULL;
}



