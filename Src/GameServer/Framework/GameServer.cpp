/*
 * GameServer.cpp
 *
 *  Created on: 2014年6月19日
 *      Author: jarek
 */

#include <Engine/Config/ConfigFile.h>
#include <Engine/Message/MessageLayer.h>
#include <Engine/Object/New0.h>
#include <Engine/Thread/ThreadManager.h>
#include <Framework/LogicThread.h>
#include <GameServer/Framework/DataBinConfig.h>
#include <GameServer/Framework/GameServer.h>
#include <GameServer/Framework/GameServerConfig.h>
#include <GameServer/Framework/GameServerModuleCenter.h>
#include <GameServer/Framework/MessageThread.h>
#include <GameServer/Message/Channel/ConnectorMessageChannel.h>
#include <GameServer/Message/Channel/DBProxyMessageChannel.h>
#include <GameServer/Message/Channel/GMConnectorMessageChannel.h>

using namespace std;


GameServer::GameServer()
{
    configFile = NULL;
    moduleCenter = NULL;
    messageLayer = NULL;
}

GameServer::~GameServer()
{
    // NULL
}

bool GameServer::LoadConfigFile(const std::string& configFilePath,
                                const LogicServerConfigItem*& outLogicServerConfigItem,
                                const ConfigFile*& outConfigFile)
{
    configFile = GameServerConfigFile::Instance();
    if (!configFile->LoadFromXml(configFilePath.c_str()))
    {
        printf("GameServer load config file <path:%s> fail", configFilePath.c_str());
        return false;
    }

    outLogicServerConfigItem = &configFile->Item();
    outConfigFile = configFile;
    return true;
}

bool GameServer::InitMessageLayer()
{
    bool successful = true;
    successful = successful && messageLayer->AddChannel(*New<ConnectorMessageChannel>());
    successful = successful && messageLayer->AddChannel(*New<DBProxyMessageChannel>());
    successful = successful && messageLayer->AddChannel(*New<GMConnectorMessageChannel>());

    return successful;
}

bool GameServer::InitThreadManager()
{
    threadManager->AddThread(*New<LogicThread>());
    threadManager->AddThread(*New<MessageThread>());
    return true;
}

bool GameServer::InitExtraComponent()
{
    // 读取配置主索引表
    string dataBinPath = configFile->GetFileDir() + configFile->Item().GameDataPath + "/Databin.xml";
    if (!DataBinConfigFile::Instance().LoadFromXml(dataBinPath.c_str()))
    {
        assert(false);
        return false;
    }

    // 初始化模块中心
    moduleCenter = GameServerModuleCenter::Instance();
    if (!moduleCenter->Initialize())
    {
        assert(false);
        return false;
    }

    return true;
}

void GameServer::StartGracefullyStop()
{
    moduleCenter->StartGracefullyStop();
}

bool GameServer::CheckGracefullyStopFinished()
{
    return moduleCenter->CheckGracefullyStopFinished();
}









