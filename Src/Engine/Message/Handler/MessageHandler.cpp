/*
 * MessageHandler.cpp
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#include <Delay/DelayFunction.h>
#include <Delay/Function/DelayDeleteObject.h>
#include <Log/LogMacro.h>
#include <Message/Handler/MessageHandler.h>
#include <Message/Message/Message.h>
#include <Object/New.h>
#include <Object/New1.h>
#include <Time/TimerCallback.h>
#include <string>

using namespace std;


MessageHandler::MessageHandler()
{
    this->message = NULL;
}

MessageHandler::~MessageHandler()
{
    Delete(message);
}

void MessageHandler::Handle(Message& msg)
{
    message = &msg;
    handleTimer.Start(TimerCallback(this, &MessageHandler::OnTimerHandle), 0);
}

void MessageHandler::OnRelease()
{
    DelayFunctionInvoker::Invoke(New<DelayDeleteObject<MessageHandler> >(this));
}

void MessageHandler::OnGetDataTimeOut()
{
    LOG_ERR("Message<%s> get data timeout", message->BriefInfo().c_str());
}

void MessageHandler::OnTimerHandle()
{
    handleTimer.Stop();
    AsyncTask::StartExecuteAsyncTask();
}



