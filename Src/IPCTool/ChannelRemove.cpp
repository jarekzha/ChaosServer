/*
 * QueueRemover.cpp
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#include <ChannelRemove.h>
#include <string>
#include <boost/interprocess/ipc/message_queue.hpp>

using namespace std;
using namespace boost::interprocess;

ChannelRemove::ChannelRemove()
{
    // NULL
}

ChannelRemove::~ChannelRemove()
{
    // NULL
}

void ChannelRemove::Execute(const IPCConfigFile& config)
{
    for (size_t i = 0; i < config.Channel.size(); ++i)
    {
        const IPCChannelConfigItem& channelConfig = config.Channel[i];
        string channelName1 = IPCChannelName(config.NamePrefix, i, channelConfig, IPCSubChannelType::Proc1to2);
        string channelName2 = IPCChannelName(config.NamePrefix, i, channelConfig, IPCSubChannelType::Proc2to1);

        RemoveChannel(channelName1);
        RemoveChannel(channelName2);
    }
}

void ChannelRemove::RemoveChannel(const std::string& channelName)
{
    bool ret = message_queue::remove(channelName.c_str());
    printf("remove channel %s %s\n", channelName.c_str(), (ret?"success":"fail"));
}

