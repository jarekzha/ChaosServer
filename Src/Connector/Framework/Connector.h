/*
 * Connector.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/TMTSingleton.h>
#include <Util/ConnectorConfig.h>
#include <string>

class TimerManager;

class RequestManager;
class IPCHandle;
class ThreadManager;


class Connector : public TMTSingleton<Connector>
{
public:
    Connector();
    virtual             ~Connector();

public:
    bool                Initialize(const std::string& configFilePath);

    void                Run();

    void                Stop();

    // accessor
public:
    const ConnectorConfigFile&  GetConfigFile();

    const ConnectorConfigItem&  GetConfigItem();

    ThreadManager&      GetThreadManager();

    IPCHandle&          GetIPCHandle();

    RequestManager&     GetRequestManager();

private:
    bool                InitializeThread();

private:
    bool                continueFlag;
    ConnectorConfigFile configFile;

    ThreadManager&      threadManager;
    IPCHandle&          ipcHandle;
    RequestManager&     requestManager;
    TimerManager&       timerManager;
};


#define theConnector (Connector::Instance())


