/*
 * GMConnectorMessageChannel.cpp
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#include <Common/Message/ProtoBuffMessage.h>
#include <Engine/Object/New.h>
#include <Engine/Factory/Factory.h>
#include <Engine/Util/TMTSingleton.h>
#include <GameServer/Framework/GameServerConfig.h>
#include <GameServer/Message/Channel/GMConnectorMessageChannel.h>
#include <GameServer/Message/Message/GMMessage.h>

GMConnectorMessageChannel::GMConnectorMessageChannel()
{
    // NULL
}

GMConnectorMessageChannel::~GMConnectorMessageChannel()
{
    // NULL
}

bool GMConnectorMessageChannel::InitConfig(Config& config)
{
    const GameServerConfigItem& gameSvrConfig = GameServerConfigFile::Instance().Item();

    config.LocalProcID = gameSvrConfig.ProcID;
    config.RemoteProcID = gameSvrConfig.DBProxyProcID;
    config.MsgCreatorFunc = Creator<GMMessage, ProtoBuffMessage>;
    return true;
}
