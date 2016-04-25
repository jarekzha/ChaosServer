#pragma once

#include <map>
#include "Engine/Util/TMTSingleton.h"
#include "DBThread.h"


class DBThreadPool : public TMTSingleton<DBThreadPool>
{
public:
                        DBThreadPool();
    virtual             ~DBThreadPool();

    bool                Initialize();

    void                InsertDBMsg(int connID, const MsgSet& msgSet);

    void                Stop();

private:
    typedef std::map<int, DBThread*>   DBThreadMap;
    DBThreadMap         dbThreadMap;
    
    int                 threadNum;
};

