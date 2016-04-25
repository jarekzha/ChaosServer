/*
 * IPCCounter.cpp
 *
 *  Created on: 2014年8月4日
 *  Author: jarek
 *  Brief:
 */

#include <IPC/IPCCounter.h>
#include <Log/LogMacro.h>
#include <Util/Util.h>

IPCCounter::IPCCounter()
{
    // NULL
}

void IPCCounter::CountSend(int remoteProcID, int dataLen)
{
    ASSERT_RETURN_VOID(dataLen >= 0);

    IPCCountData& count = countDataMap[remoteProcID];
    count.SendLen += dataLen;
    count.SendMsgCount++;
}

void IPCCounter::CountRecv(int remoteProcID, int dataLen)
{
    ASSERT_RETURN_VOID(dataLen >= 0);

    IPCCountData& count = countDataMap[remoteProcID];
    count.RecvLen += dataLen;
    count.RecvMsgCount++;
}

void IPCCounter::Print(int selfProcID)
{
    for (ProcIDCountDataMap::iterator iter = countDataMap.begin();
            iter != countDataMap.end(); ++iter)
    {
        IPCCountData& countData = iter->second;
        LOG_RUN("SelfProc<ID:%d> remoteProc<ID:%d> send <len:%d, msgCount:%d> recv <len:%d, msgCount:%d>",
                selfProcID, iter->first,
                countData.SendLen, countData.SendMsgCount,
                countData.RecvLen, countData.RecvMsgCount);
    }
}




