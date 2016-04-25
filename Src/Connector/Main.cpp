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
#include <Framework/Connector.h>
#include <getopt.h>
#include <signal.h>
#include <climits>
#include <cstring>
#include <string>

void SigStop(int)
{
    theConnector.Stop();
}

int main(int argc, char* argv[])
{
    const char* optionDes = "c:";
    char configPath[PATH_MAX] = "./Config/ConnectorConfig.xml";

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

    LogGlobalParam::Instance().LogTag = "Connector";
    LogRun::Instance().Initialize((char*)"./Log", "Connector_Run");
    LogErr::Instance().Initialize((char*)"./Log", "Connector_Err");
    LogTrace::Instance().Initialize((char*)"./Log", "Connector_Trace");
    InitProtobufLogErr();

    FCGX_Init();

    if (!theConnector.Initialize(configPath))
    {
        LOG_ERR("Connector initialize failed");
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);
    signal(SIGUSR1, SigStop);
    InitSignalHandler();

    theConnector.Run();

    Time::MsSleep(1000);       // 为了保证最后的日志可以写入磁盘

    LogRun::Instance().CloseLog();
    LogErr::Instance().CloseLog();
    LogTrace::Instance().CloseLog();

    return 0;
}

