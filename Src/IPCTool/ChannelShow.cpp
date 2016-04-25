/*
 * ChannelShow.cpp
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#include <ChannelShow.h>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <string>
#include <stdio.h>

using namespace std;
using namespace boost::interprocess;

ChannelShow::ChannelShow()
{
    // NULL
}

ChannelShow::~ChannelShow()
{
    // NULL
}

void ChannelShow::Execute(const IPCConfigFile& config)
{
    for (size_t i = 0; i < config.Channel.size(); ++i)
    {
        const IPCChannelConfigItem& channelConfig = config.Channel[i];
        string channelName1 = IPCChannelName(config.NamePrefix, i, channelConfig, IPCSubChannelType::Proc1to2);
        string channelName2 = IPCChannelName(config.NamePrefix, i, channelConfig, IPCSubChannelType::Proc2to1);

        ShowChannel(channelName1);
        ShowChannel(channelName2);
    }
}

void ChannelShow::ShowChannel(std::string& name)
{
    message_queue* queue;
    try
    {
        queue = new message_queue(open_only, name.c_str());
    }
    catch (exception& e)
    {
        printf("Open channel %s fail:%s\n", name.c_str(), e.what());
        return;
    }

    printf("channel:%s ---- msgnum:%d\n", name.c_str(), (int)queue->get_num_msg());

    delete queue;
}
