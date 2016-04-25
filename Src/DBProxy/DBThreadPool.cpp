#include <DBProxy/DBThreadPool.h>
#include <DBProxy/Util/DBProxyConfig.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Engine/Object/New.h>

class Msg;

DBThreadPool::DBThreadPool()
{
    dbThreadMap.clear();
    threadNum = 0;
}

DBThreadPool::~DBThreadPool()
{
    DBThreadMap::iterator iter = dbThreadMap.begin();
    for (;iter != dbThreadMap.end(); ++iter)
    {
        Delete(iter->second);
    }

    dbThreadMap.clear();
    threadNum = 0;
}

bool DBThreadPool::Initialize()
{
    const DBProxyConfigItem& dbProxyConfig = DBProxyConfigFile::Instance().Item();
    threadNum = dbProxyConfig.ThreadNum;

    for (int i = 1; i <= threadNum; i++ )
    {
        DBThread* dbThread = New<DBThread>(i);
        if (!dbThread->Initialize())
        {
            return false;
        }

        dbThreadMap.insert(DBThreadMap::value_type(i, dbThread));
    }
    
    return true;
}

void DBThreadPool::InsertDBMsg(int connID, const MsgSet& msgSet)
{
    DBThreadMap::iterator iter = dbThreadMap.find(connID);
    if (iter == dbThreadMap.end())
    {
        LOG_ERR("Cannot find thread <conn:%d>", connID);
        assert(false);
        return;
    }

    DBThread* dbThread = iter->second;

    dbThread->InsertMsg(msgSet);
}

void DBThreadPool::Stop()
{
    DBThreadMap::iterator iter = dbThreadMap.begin();
    for (; iter != dbThreadMap.end(); ++iter)
    {
        DBThread* dbThread = iter->second;
        dbThread->Stop();
    }
}

