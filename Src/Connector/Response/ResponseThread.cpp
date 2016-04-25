/*
 * ResponseThread.cpp
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Delay/DelayCenter.h>
#include <Engine/Time/Time.h>
#include <Engine/Time/TimerManager.h>
#include <Engine/Util/TMTSingleton.h>
#include <Response/ResponseThread.h>


ResponseThread::ResponseThread()
{
    // NULL
}

ResponseThread::~ResponseThread()
{
    // NULL
}

void ResponseThread::Routine()
{
    Time::UpdateTime();
    responseHandler.HandleResponse();
    TimerManager::Instance().Run();
    DelayCenter::Instance().InvokeDelayFunction();
}
