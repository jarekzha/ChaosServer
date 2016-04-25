#pragma once

#include <limits.h>
#include <string>
#include "Engine/Util/Singleton.h"
#include "Engine/Log/LogMacro.h"
#include "Engine/Config/ConfigItem.h"
#include "Engine/Config/ConfigFile.h"

class GMConnectorConfigItem: public ConfigItem
{
public:
    int ProcID;                 // 用于IPC的进程别名
    std::string IP;                     // IP地址
    int Port;                   // 端口号
    int RequestCount;           // 共存的请求数
    std::string IPCConfig;              // IPC配置路径
    int GameServerProcID;       // GameServer的进程ID
    std::string HTTPUrl;        //http url

    virtual bool LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_VALUE(element, "ProcID", ProcID);
        LOAD_XML_STRING(element, "IP", IP);
        LOAD_XML_VALUE(element, "Port", Port);
        LOAD_XML_VALUE(element, "RequestCount", RequestCount);
        LOAD_XML_STRING(element, "IPCConfig", IPCConfig);
        LOAD_XML_VALUE(element, "GameServerProcID", GameServerProcID);
        LOAD_XML_STRING(element, "HTTPUrl", HTTPUrl);
        return true;
    }

    void DisplayConfig()
    {
        LOG_RUN("ProcID:%d", ProcID);
        LOG_RUN("IP:%s", IP.c_str());
        LOG_RUN("Port:%d", Port);
        LOG_RUN("RequestCount:%d", RequestCount);
        LOG_RUN("IPCConfig:%s", IPCConfig.c_str());
        LOG_RUN("GameServerProcID:%d", GameServerProcID);
        LOG_RUN("HTTPUrl:%s", HTTPUrl.c_str());
    }
};

class GMConnectorConfigFile: public ConfigFileSingleItem<GMConnectorConfigItem>
{
public:
    // null
};

