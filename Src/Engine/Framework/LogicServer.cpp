/*
 * LogicServer.cpp
 *
 *  Created on: 2014年6月17日
 *      Author: jarek
 */

#include <Config/ConfigFile.h>
#include <Delay/DelayCenter.h>
#include <Framework/LogicServer.h>
#include <Framework/LogicServerConfig.h>
#include <Log/LogGlobalParam.h>
#include <Log/LogMacro.h>
#include <Log/LogType.h>
#include <Message/MessageLayer.h>
#include <Thread/ThreadManager.h>
#include <Time/Time.h>
#include <Time/TimerManager.h>
#include <Util/FakeRandom.h>
#include <Util/OnlyOne.h>
#include <Util/TMTSingleton.h>
#include <IPC/IPCHandle.h>
#include <Util/ProtoBuf.h>
#include <cassert>
#include <ctime>

using namespace std;


LogicServer::LogicServer()
{
    continueFlag = true;

    configFile = NULL;
    configItem = NULL;

    // 组件
    fakeRandom = &FakeRandom::Instance();
    delayCenter = &DelayCenter::Instance();
    timerManager = &TimerManager::Instance();
    threadManager = &ThreadManager::Instance();
    ipcHandle = &IPCHandle::Instance();
    messageLayer = &MessageLayer::Instance();
}

LogicServer::~LogicServer()
{
    // NULL
}

bool LogicServer::Initialize(const string& serverName, const string& configFileFullPath, bool deamon)
{
    name = serverName;
    configFilePath = configFileFullPath;

    // 配置
    if (LoadConfigFile(configFilePath, configItem, configFile)
            && configItem != NULL
            && configFile != NULL)
    {
        LOG_RUN("LogicServer <name:%s> load config file <path:%s> success", name.c_str(), configFileFullPath.c_str());
    }
    else
    {
        LOG_ERR("Load config file <path:%s> fail", configFilePath.c_str());
        return false;
    }

    // deamon
    if (deamon)
    {
        OnlyOne onlyone(name.c_str(), configItem->ProcID);
        if (onlyone.OnlyOneDaemon(1, 1))
        {
            LOG_RUN("LogicServer <name:%s> OnlyOneDaemon success", name.c_str());
        }
        else
        {
            LOG_ERR("LogicServer <name:%s> OnlyOneDaemon fail", name.c_str());
            return false;
        }
    }

    // 初始化LOG
    LogGlobalParam::Instance().LogTag = name;
    LogRun::Instance().Initialize((char*)"./Log", (char*)(name + "_Run").c_str());
    LogErr::Instance().Initialize((char*)"./Log", (char*)(name + "_Err").c_str());
    LogTrace::Instance().Initialize((char*)"./Log", (char*)(name + "_Trace").c_str());
    LogWarn::Instance().Initialize((char*)"./Log", (char*)(name + "_Warn").c_str());
    InitProtobufLogErr();

    // 输出配置
    configItem->DisplayConfig();

    // 基础组件
    if (InitBaseComonent())
    {
        LOG_RUN("LogicServer <name:%s> init base component success", name.c_str());
    }
    else
    {
        LOG_ERR("LogicServer init base component fail");
        return false;
    }

    // 附加组件
    if (InitExtraComponent())
    {
        LOG_RUN("LogicServer <name:%s> init extra component success", name.c_str());
    }
    else
    {
        LOG_ERR("LogicServer init extra component fail");
        return false;
    }

    LOG_RUN("LogicServer <name:%s> initialize successfully", name.c_str());

    return true;
}

bool LogicServer::InitBaseComonent()
{
    // 设置随机种子
    fakeRandom->SetSeed(time(NULL));

    // 初始化时间
    Time::UpdateTime();

    // 初始化IPC
    string ipcConfigFile = configFile->GetFileDir() + configItem->IPCConfigFile;
    if (!ipcHandle->Initialize(ipcConfigFile, configItem->ProcID))
    {
        LOG_ERR("LogicServer <name:%s> init ipc handle fail", name.c_str());
        assert(false);
        return false;
    }

    // 初始化定时器
    if (!timerManager->Initialize(configItem->TimerMSecondPerTick,
                                  configItem->TimerWheelNum,
                                  configItem->TimerBucketNumPerWheel))
    {
        LOG_ERR("LogicServer <name:%s> init timer fail", name.c_str());
        assert(false);
        return false;
    }

    if (!InitMessageLayer())
    {
        LOG_ERR("LogicServer <name:%s> init message layer fail", name.c_str());
        assert(false);
        return false;
    }

    if (!InitThreadManager())
    {
        LOG_ERR("LogicServer <name:%s> init thread manager fail", name.c_str());
        assert(false);
        return false;
    }

    return true;
}

void LogicServer::Run()
{
    LOG_RUN("LogicServer <name:%s> run", name.c_str());
    threadManager->Start();

    // 运行时
    while (continueFlag)
    {
        Time::MsSleep(1000);
    }

    // 优雅停止
    while (!CheckGracefullyStopFinished())
    {
        Time::MsSleep(500);
    }

    threadManager->Stop();

    Time::MsSleep(1000);       // 为了保证最后的日志可以写入磁盘
    LogRun::Instance().CloseLog();
    LogErr::Instance().CloseLog();
    LogTrace::Instance().CloseLog();
    LogWarn::Instance().CloseLog();
}

void LogicServer::Stop()
{
    continueFlag = false;
    LOG_RUN("LogicServer <name:%s> start stopping", name.c_str());

    StartGracefullyStop();
}

const std::string& LogicServer::GetName()
{
    return name;
}




