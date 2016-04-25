/*
 * GMConnector.cpp
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/IPC/IPCHandle.h>
#include <Engine/Log/LogMacro.h>
#include <GMConnector.h>
#include "HTTPRequest.h"
#include <cassert>
#include <cstdio>

using namespace std;

GMConnector::GMConnector() :
        ipcHandle(IPCHandle::Instance()), httpRequest(NULL)
{
    continueFlag = true;

    httpRequest = New<HTTPRequest>();
}

GMConnector::~GMConnector()
{
    Delete(httpRequest);
}

bool GMConnector::Initialize(const std::string& configFilePath)
{
    if (!configFile.LoadFromXml(configFilePath.c_str()))
    {
        LOG_ERR("GMConnector load configfile<path:%s> fail", configFilePath.c_str());
        assert(false);
        return false;
    }

    const GMConnectorConfigItem& configItem = configFile.Item();
    string ipcConfigFile = configFile.GetFileDir() + configItem.IPCConfig;
    if (!ipcHandle.Initialize(ipcConfigFile, configItem.ProcID))
    {
        LOG_ERR("GMConnector ipc handle initialize fail<%s>", configItem.IPCConfig.c_str());
        assert(false);
        return false;
    }

    if (NULL == httpRequest || !httpRequest->Initialize())
    {
        LOG_ERR("GMConnector http-request initialize fail");
        assert(false);
        return false;
    }

    LOG_RUN("GMConnector initialize successfully");

    return true;
}

void GMConnector::Run()
{
    LOG_RUN("GMConnector run");

    while (continueFlag)
    {
        if (!httpRequest->DispatchHTTPRequest())
        {
            Time::MsSleep(100);
        }
    }
}

void GMConnector::Stop()
{
    continueFlag = false;
    LOG_RUN("GMConnector stop");
}

IPCHandle& GMConnector::GetIPCHandle()
{
    return ipcHandle;
}

const GMConnectorConfigFile& GMConnector::GetConfigFile()
{
    return configFile;
}

const GMConnectorConfigItem& GMConnector::GetConfigItem()
{
    return configFile.Item();
}

