/*
 * Connector.cpp
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/IPC/IPCHandle.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Thread/ThreadManager.h>
#include <Engine/Time/Time.h>
#include <Engine/Time/TimerManager.h>
#include <Framework/Connector.h>
#include <Request/RequestManager.h>
#include <Request/RequestThread.h>
#include <Response/ResponseThread.h>
#include <cassert>
#include <cstdio>

using namespace std;


Connector::Connector()
    : threadManager(ThreadManager::Instance())
    , ipcHandle(IPCHandle::Instance())
    , requestManager(RequestManager::Instance())
    , timerManager(TimerManager::Instance())
{
    continueFlag = true;
}

Connector::~Connector()
{
}

bool Connector::Initialize(const std::string& configFilePath)
{
    if (!configFile.LoadFromXml(configFilePath.c_str()))
    {
        LOG_ERR("Connector load configfile<path:%s> fail",
                configFilePath.c_str());
        assert(false);
        return false;
    }

    const ConnectorConfigItem& configItem = configFile.Item();
    string ipcConfigFile = configFile.GetFileDir() + configItem.IPCConfig;
    if (!ipcHandle.Initialize(ipcConfigFile, configItem.ProcID))
    {
        LOG_ERR("Connector ipc handle initialize fail<%s>", configItem.IPCConfig.c_str());
        assert(false);
        return false;
    }

    if (!timerManager.Initialize())
    {
        LOG_ERR("Connector timer manager initialize fail");
        assert(false);
        return false;
    }

    if (!requestManager.Initialize(configFile.Item().RequestCount))
    {
        LOG_ERR("Connector request manager initialize fail");
        return false;
    }

    if (!InitializeThread())
    {
        return false;
    }

    LOG_RUN("Connector initialize successfully");

    return true;
}

void Connector::Run()
{
    LOG_RUN("Connector run");

    threadManager.Start();

    while (continueFlag)
    {
        Time::MsSleep(1000);
    }

    threadManager.Stop();
}

void Connector::Stop()
{
    continueFlag = false;
    LOG_RUN("Connector stop");
}

ThreadManager& Connector::GetThreadManager()
{
    return threadManager;
}

IPCHandle& Connector::GetIPCHandle()
{
    return ipcHandle;
}

RequestManager& Connector::GetRequestManager()
{
    return requestManager;
}

const ConnectorConfigFile& Connector::GetConfigFile()
{
    return configFile;
}

const ConnectorConfigItem& Connector::GetConfigItem()
{
    return configFile.Item();
}

bool Connector::InitializeThread()
{
    threadManager.AddThread(*New<RequestThread>());
    threadManager.AddThread(*New<ResponseThread>());
    return true;
}

