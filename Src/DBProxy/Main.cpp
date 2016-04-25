/*
*   DB的代理服务器，用mysql connector/c++ 1.1.0 实现
*/

#include <DBMainLoop.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/Log/LogGlobalParam.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Log/LogType.h>
#include <Engine/Time/Time.h>
#include <Engine/Util/OnlyOne.h>
#include <Engine/Util/ProtoBuf.h>
#include <Engine/Util/SignalHandler.h>
#include <Engine/Util/TMTSingleton.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <Util/DBProxyConfig.h>
#include <climits>
#include <string>

int main(int argc, char* argv[])
{
    const char* optionDes = "c:d";
    char configPath[PATH_MAX] = "./Config/DBProxyConfig.xml";
    bool deamon = false;

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"deamon", no_argument, 0, 'd'},
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
            case 'd':
                deamon = true;
                break;
        }
    }

    DBProxyConfigFile& configFile = DBProxyConfigFile::Instance();
    if (!configFile.LoadFromXml(configPath))
    {
        LOG_ERR("error: loadConfig failed!");
        return -1;
    }

    if (deamon)
    {
        OnlyOne onlyone("DBProxy", configFile.Item().ProcID);
        if (!onlyone.OnlyOneDaemon(1, 1))
        {
            return -1;
        }
    }

    LogGlobalParam::Instance().LogTag = "DBProxy";
    LogRun::Instance().Initialize((char*)"./Log", "DBProxy_Run");
    LogErr::Instance().Initialize((char*)"./Log", "DBProxy_Err");
    LogTrace::Instance().Initialize((char*)"./Log", "DBProxy_Trace");
    InitProtobufLogErr();

    configFile.Item().DisplayConfig();

    InitSignalHandler();

    DBMainLoop mainLoop;
    if (mainLoop.Initialize())
    {
        LOG_RUN("Initialize Success!");
        mainLoop.Run();
    }
    else
    {
        LOG_ERR("Main loop initialize failed!");
    }

    LOG_RUN("%s is shutdown!", argv[0]);

    Time::MsSleep(1000);       // 为了保证最后的日志可以写入磁盘

    LogRun::Instance().CloseLog();
    LogErr::Instance().CloseLog();
    LogTrace::Instance().CloseLog();
    
    return 0;
}

