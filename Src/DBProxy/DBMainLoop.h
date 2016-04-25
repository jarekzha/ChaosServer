/*
*   数据库进程的MainLoop
*/
#pragma once

#include <map>
#include <Protocol/NetMsg.pb.h>
#include <DBThread.h>

class DBMainLoop
{
public:
                        DBMainLoop();
                        ~DBMainLoop();

    static void         ExitFlag(int);

    bool                Initialize();

    void                Run();

    bool                ReceiveIPCMsg(MsgSet& msgSet);
    
private:
    static volatile bool continueFlag;      // 加volatile避免编译器优化
    
    int                 threadNum;
};

