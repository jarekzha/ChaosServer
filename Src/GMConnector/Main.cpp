/*
 * Main.cpp fastCGI程序
 */
#include <Engine/IPC/IPCHandle.h>
#include <Engine/Log/LogGlobalParam.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Log/LogType.h>
#include <Engine/Time/Time.h>
#include <Engine/Util/ProtoBuf.h>
#include <Engine/Util/SignalHandler.h>
#include <Engine/Util/TMTSingleton.h>
#include <fcgiapp.h>
#include <GMConnector.h>
#include <getopt.h>
#include <signal.h>
#include <climits>
#include <cstring>
#include <string>

void SigStop(int)
{
    theGMConnector.Stop();
}

int main(int argc, char* argv[])
{
    const char* optionDes = "c:";
    char configPath[PATH_MAX] = "./Config/GMConnectorConfig.xml";

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    while(1)
    {
        int option_index = 0;
        int c = getopt_long(argc, argv, optionDes, long_options, &option_index);
        if (-1 == c)
            break;

        switch(c)
        {
            case 'c':
                strncpy(configPath, optarg, sizeof(configPath));
                break;
        }
    }

    LogGlobalParam::Instance().LogTag = "GMConnector";
    LogRun::Instance().Initialize((char*)"./Log", "GMConnector_Run");
    LogErr::Instance().Initialize((char*)"./Log", "GMConnector_Err");
    LogTrace::Instance().Initialize((char*)"./Log", "GMConnector_Trace");
    InitProtobufLogErr();

    FCGX_Init();

    if (!theGMConnector.Initialize(configPath))
    {
        LOG_ERR("GMConnector initialize failed");
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGUSR1, SigStop);
    InitSignalHandler();

    theGMConnector.Run();

    Time::MsSleep(1000);       // 为了保证最后的日志可以写入磁盘

    LogRun::Instance().CloseLog();
    LogErr::Instance().CloseLog();
    LogTrace::Instance().CloseLog();

    return 0;
}

