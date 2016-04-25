#include <signal.h>
#include <DBProxy/DBMainLoop.h>
#include <DBProxy/Util/DBProxyConfig.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Time/Time.h>
#include <DBProxy/DBThreadPool.h>
#include <Engine/IPC/IPCHandle.h>

volatile bool DBMainLoop::continueFlag = true;

DBMainLoop::DBMainLoop()
{
    signal(SIGUSR1, DBMainLoop::ExitFlag);
    threadNum = 0;
}

DBMainLoop::~DBMainLoop()
{
}

void DBMainLoop::ExitFlag(int)
{
    continueFlag = false;
    LOG_RUN("Receive SIGUSR1, shutdown!");
}

bool DBMainLoop::Initialize()
{
    const DBProxyConfigItem& dbProxyConfig = DBProxyConfigFile::Instance().Item();
    IPCHandle& ipcHandle = IPCHandle::Instance();

    threadNum = dbProxyConfig.ThreadNum;

    bool ret = ipcHandle.Initialize(dbProxyConfig.IPCConfig, dbProxyConfig.ProcID);
    if (!ret)
    {
        LOG_ERR("IPCHandle Initialize failed!");
        return false;
    }

    DBThreadPool& dbThreadPool = DBThreadPool::Instance();
    if (!dbThreadPool.Initialize())
    {
        LOG_ERR("DBThread Pool Initialize failed!");
        return false;
    }
        
    return true;
}

void DBMainLoop::Run()
{    
    MsgSet msgSet;
    DBThreadPool& dbThreadPool = DBThreadPool::Instance();

    while (continueFlag)
    {
        Time::UpdateTime();
        
        msgSet.Clear();
        
        if (ReceiveIPCMsg(msgSet))
        {
            //这里用sessionID根据线程数量进行取模分配到具体的DB线程
            int modID = msgSet.head().session_id() % threadNum;

            ASSERT_BREAK(msgSet.msg().size() == 1);

            dbThreadPool.InsertDBMsg(modID+1, msgSet);
        }

        Time::MsSleep(10);
    }

    dbThreadPool.Stop();
}

bool DBMainLoop::ReceiveIPCMsg(MsgSet& msgSet)
{
    IPCHandle& ipcHandle = IPCHandle::Instance();
    IPCHandleRes ret = ipcHandle.GetCompleteMessage(msgSet);
    if (IPCHandleRes_Error == ret)
    {
        LOG_ERR("IPCHandle Error! error");
        return false;
    }

    return IPCHandleRes_Success == ret;
}

