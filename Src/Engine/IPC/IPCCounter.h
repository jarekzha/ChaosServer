/*
 * IPCCounter.h
 *
 *  Created on: 2014年8月4日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <map>

struct IPCCountData
{
    IPCCountData():SendLen(0), SendMsgCount(0), RecvLen(0), RecvMsgCount(0){}

    int                 SendLen;
    int                 SendMsgCount;
    int                 RecvLen;
    int                 RecvMsgCount;
};

class IPCCounter
{
public:
    IPCCounter();

public:
    void                CountSend(int remoteProcID, int dataLen);

    void                CountRecv(int remoteProcID, int dataLen);

    void                Print(int selfProcID);

private:
    typedef std::map<int, IPCCountData> ProcIDCountDataMap;
    ProcIDCountDataMap  countDataMap;
};
