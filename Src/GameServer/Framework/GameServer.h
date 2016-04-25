/*
 * GameServer.h
 *
 *  Created on: 2014年6月18日
 *      Author: jarek
 */

#pragma once

#include <Engine/Framework/LogicServer.h>
#include <Engine/Util/TMTSingleton.h>
#include <string>

class GameServerConfigFile;
class GameServerModuleCenter;
class GameServerMessageLayer;

/**
 * 游戏服务器
 */
class GameServer : public LogicServer
                 , public TMTSingleton<GameServer>
{
public:
    GameServer();
    virtual             ~GameServer();

// LogicServerExtendable
private:
    // 载入配置文件
    virtual bool        LoadConfigFile(const std::string& configFilePath,
                                       const LogicServerConfigItem*& outLogicServerConfigItem,
                                       const ConfigFile*& outConfigFile);

    // 初始化消息层
    virtual bool        InitMessageLayer();

    // 初始化线程管理器
    virtual bool        InitThreadManager();

    // 初始化扩展组件
    virtual bool        InitExtraComponent();

    // GracefullyStopable
private:
    // 开始优雅停止
    virtual void        StartGracefullyStop();

    // 检测优雅停止是否结束
    virtual bool        CheckGracefullyStopFinished();

private:
    GameServerConfigFile*   configFile;
    GameServerModuleCenter* moduleCenter;
};


#define theGameServer  GameServer::Instance()


