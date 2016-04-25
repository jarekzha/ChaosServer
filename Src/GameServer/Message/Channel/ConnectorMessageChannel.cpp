/*
 * ConnectorMessageChannel.cpp
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Factory/Factory.h>
#include <Engine/Util/TMTSingleton.h>
#include <GameServer/Framework/GameServerConfig.h>
#include <GameServer/Message/Channel/ConnectorMessageChannel.h>
#include <GameServer/Message/Message/CSRequest.h>

using namespace std;

ConnectorMessageChannel::ConnectorMessageChannel()
{
    // NULL
}

ConnectorMessageChannel::~ConnectorMessageChannel()
{
    // NULL
}

bool ConnectorMessageChannel::InitConfig(Config& config)
{
    const GameServerConfigItem& gameSvrConfig = GameServerConfigFile::Instance().Item();

    config.LocalProcID = gameSvrConfig.ProcID;
    config.RemoteProcID = gameSvrConfig.ConnectorProcID;
    config.MsgCreatorFunc = Creator<CSRequest, ProtoBuffMessage>;
    return true;
}





