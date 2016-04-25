#include <Engine/Framework/LogicServer.h>
#include <Engine/IPC/IPCHandle.h>
#include <Framework/GameServerModuleCenter.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/SignalHandler.h>
#include <Engine/Util/TMTSingleton.h>
#include <Framework/GameServer.h>
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <climits>
#include <string>


void SigStop(int)
{
    theGameServer.Stop();
}

void SigReloadConfig(int)
{
    GameServerModuleCenter::Instance().ReloadConfigModule();
}

int main(int argc, char* argv[])
{
    const char* optionDes = "c:d";
    char configPath[PATH_MAX] = "./Config/GameServerConfig.xml";
    bool deamon = false;

    static struct option long_options[] = {
        {"config", required_argument, 0, 'c'},
        {"deamon", no_argument, 0, 'd'},
        {0, 0, 0, 0}
    };

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
            case 'd':
                deamon = true;
                break;
        }
    }

    signal(SIGUSR1, SigStop);
    signal(SIG_RELOAD_CONFIG, SigReloadConfig);
    InitSignalHandler();

    if (!theGameServer.Initialize("GameServer", configPath, deamon))
    {
        LOG_ERR("GameServer initialize fail");
        return false;
    }
    else
    {
        LOG_RUN("GameServer initlialize success");
    }
    
    theGameServer.Run();

    LOG_RUN("%s is shutdown!", theGameServer.GetName().c_str());

    return 0;
}


