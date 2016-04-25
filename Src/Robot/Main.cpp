/*
 * Main.cpp
 *
 *  Created on: 2014年7月26日
 *  Author: jarek
 *  Brief: 
 */


#include <Engine/Log/LogMacro.h>
#include <Engine/Time/Time.h>
#include <Engine/Util/ProtoBuf.h>
#include <Framework/Robot.h>
#include <getopt.h>
#include <climits>
#include <cstring>

int main(int argc, char* argv[])
{
    const char* optionDes = "c:";
    char configPath[PATH_MAX] = "./Config/RobotConfig.xml";

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
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

    LogGlobalParam::Instance().LogTag = "Robot";
    LogRun::Instance().Initialize((char*)"./Log", "Robot_Run");
    LogErr::Instance().Initialize((char*)"./Log", "Robot_Err");
    LogTrace::Instance().Initialize((char*)"./Log", "Robot_Trace");
    InitProtobufLogErr();

    if (!theRobot.Initialize(configPath))
    {
        LOG_ERR("Robot initialize failed");
        return 1;
    }

    theRobot.Run();

    Time::MsSleep(1000);       // 为了保证最后的日志可以写入磁盘

    LogRun::Instance().CloseLog();
    LogErr::Instance().CloseLog();
    LogTrace::Instance().CloseLog();

    return 0;
}
