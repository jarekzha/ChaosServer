/*
 * LogicServer.h
 *
 *  Created on: 2014年6月17日
 *      Author: jarek
 *
 *     逻辑服务器，通用框架
 */

#pragma once

#include <Framework/GracefullyStopable.h>
#include <Framework/LogicServerExtendable.h>
#include <string>

class IPCHandle;
class FakeRandom;
class MessageHandleHelper;
class ShmObjectManager;
class ShmContainerManager;
class LogicServerConfigItem;
class DelayCenter;
class TimerManager;
class ThreadManager;
class MessageLayer;

/**
 * 逻辑服务器通用模板
 */
class LogicServer
        : public LogicServerExtendable
        , public GracefullyStopable
{
public:
                        LogicServer();
    virtual             ~LogicServer();

    bool                Initialize(const std::string& serverName,
                                   const std::string& configFileFullPath,
                                   bool deamon);

    void                Run();

    void                Stop();

// getter
public:
    const std::string&  GetName();

private:
    bool                InitBaseComonent();

private:
    volatile bool       continueFlag;

protected:
    std::string                     name;                   // 服务器名称
    std::string                     configFilePath;         // 配置文件完整路径
    const ConfigFile*               configFile;             // 配置文件
    const LogicServerConfigItem*    configItem;             // 配置项

    // 逻辑服务器所拥有的基础组件，下面都是singleton
    FakeRandom*             fakeRandom;             // 随机数生成器
    DelayCenter*            delayCenter;            // 异步中心
    TimerManager*           timerManager;           // 定时器
    ThreadManager*          threadManager;          // 线程管理器
    IPCHandle*              ipcHandle;              // 进程间通信处理
    MessageLayer*           messageLayer;           // 消息层
};
