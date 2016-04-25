/*
 * DBProxyMessageChannel.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Factory/Factory.h>
#include <Engine/Util/TMTSingleton.h>
#include <GameServer/Framework/GameServerConfig.h>
#include <GameServer/Message/Channel/DBProxyMessageChannel.h>
#include <GameServer/Message/Message/GameServerMessage.h>

using namespace std;

DBProxyMessageChannel::DBProxyMessageChannel()
{
    // NULL
}

DBProxyMessageChannel::~DBProxyMessageChannel()
{
    // NULL
}


bool DBProxyMessageChannel::InitConfig(Config& config)
{
    const GameServerConfigItem& gameSvrConfig = GameServerConfigFile::Instance().Item();

    config.LocalProcID = gameSvrConfig.ProcID;
    config.RemoteProcID = gameSvrConfig.DBProxyProcID;
    config.MsgCreatorFunc = Creator<GameServerMessage, ProtoBuffMessage>;
    return true;
}






