/*
 * GMConnector.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/TMTSingleton.h>
#include <GMConnectorConfig.h>
#include <string>


class IPCHandle;
class HTTPRequest;
class GMConnector: public TMTSingleton<GMConnector>
{
public:
    GMConnector();
    virtual ~GMConnector();

public:
    bool Initialize(const std::string& configFilePath);

    void Run();

    void Stop();

    // accessor
public:
    const GMConnectorConfigFile& GetConfigFile();

    const GMConnectorConfigItem& GetConfigItem();

    IPCHandle& GetIPCHandle();

private:
    bool continueFlag;
    GMConnectorConfigFile configFile;

    IPCHandle& ipcHandle;

    HTTPRequest*    httpRequest;
};

#define theGMConnector (GMConnector::Instance())

