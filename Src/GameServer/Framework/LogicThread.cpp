/*
 * LogicThread.cpp
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Delay/DelayCenter.h>
#include <Engine/Message/MessageLayer.h>
#include <Engine/Time/Time.h>
#include <Engine/Time/TimerManager.h>
#include <Engine/Util/TMTSingleton.h>
#include <GameServer/Framework/LogicThread.h>


void LogicThread::Routine()
{
    static TimerManager& timerManager = TimerManager::Instance();
    static DelayCenter& delayCenter = DelayCenter::Instance();
    static MessageLayer& messageLayer = MessageLayer::Instance();

    Time::UpdateTime();
    messageLayer.HandleAllMessage();
    timerManager.Run();
    delayCenter.InvokeDelayFunction();
}

