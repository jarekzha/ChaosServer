/*
 * SignalHandler.cpp
 *
 *  Created on: 2014年8月13日
 *  Author: jarek
 *  Brief: 
 */

#include <Util/TMTSingleton.h>
#include <IPC/IPCHandle.h>
#include <Object/ObjectCounter.h>
#include <signal.h>

void SigIPCHandlePrint(int)
{
    IPCHandle::Instance().PrintInfo();
}

void SigObjectCounterPrint(int)
{
    ObjectCounter::Instance().Print();
}


void InitSignalHandler()
{
    signal(SIGRTMIN, SigIPCHandlePrint);
    signal(SIGRTMIN+1, SigObjectCounterPrint);
}
