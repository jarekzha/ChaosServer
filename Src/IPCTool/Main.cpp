/*
 * Main.cpp
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/Time/Time.h>
#include <getopt.h>
#include <climits>
#include <cstring>
#include <ChannelRemove.h>
#include <ChannelShow.h>
#include <Engine/Util/ProtoBuf.h>

#define DEFAULT_CONFIG_PATH     "./Config/IPCConfig.xml"

void ShowHelp()
{
    printf("Usage: IPCTool [-c configPath] -h|-d|-s\n");
    printf("Get help: IPCTool -h\n");
    printf("Some operation on ipc channel. More than one operation execute in once command is not supported.\n\n");
    printf("  -c, --config              config file path(default %s)\n", DEFAULT_CONFIG_PATH);
    printf("  -h, --help                show usage\n");
    printf("  -d, --delete              delete ipc channel defined in config file\n");
    printf("  -s, --show                show the informantion of ipc channel defined in config file\n");
    printf("\n");
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        ShowHelp();
        return 0;
    }

    const char* optionDes = "c:hds";
    char configPath[PATH_MAX] = DEFAULT_CONFIG_PATH;

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {"delete", no_argument, 0, 'd'},
        {"show", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    Executable* executale = NULL;

    while(1)
    {
        int option_index = 0;
        int c = getopt_long(argc, argv, optionDes, long_options, &option_index);
        if (-1 == c)
            break;

        switch(c)
        {
            case 'c':
                strncpy(configPath, optarg, sizeof(configPath));
                break;
            case 'h':
                ShowHelp();
                return 0;
            case 'd':
                executale = new ChannelRemove();
                break;
            case 's':
                executale = new ChannelShow();
                break;
        }
    }

    IPCConfigFile config;
    if (!config.LoadFromXml(configPath))
    {
        fprintf(stderr, "load config file:%s fail\n", configPath);
        return 1;
    }

    if (NULL == executale)
    {
        fprintf(stderr, "no executable command\n");
        return 2;
    }

    executale->Execute(config);

    return 0;
}


