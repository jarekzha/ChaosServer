/*
* DB代理服务器的配置对象
*/
#pragma once
    
#include <string>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Config/ConfigItem.h>
#include <Engine/Config/ConfigFile.h>


class DBProxyConfigItem : public ConfigItem
{
public:
    DBProxyConfigItem()
    {
        ProcID = 0;
        GameServerProcID = 0;
        ThreadNum = 0;
    }

    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_VALUE(element,    "ProcID",              ProcID);
        LOAD_XML_STRING(element,   "IPCConfig",           IPCConfig);
        LOAD_XML_VALUE(element,    "GameServerProcID",    GameServerProcID);
        LOAD_XML_STRING(element,   "DBHost",              DBHost);
        LOAD_XML_STRING(element,   "DBUserName",          DBUserName);
        LOAD_XML_STRING(element,   "DBPassword",          DBPassword);
        LOAD_XML_STRING(element,   "DBName",              DBName);
        LOAD_XML_VALUE(element,    "ThreadNum",           ThreadNum);
        return true;
    }

    void                DisplayConfig() const
    {
        LOG_RUN("ProcID:%d",            ProcID);
        LOG_RUN("IPCConfig:%s",         IPCConfig.c_str());
        LOG_RUN("GameServerProcID:%d",  GameServerProcID);
        LOG_RUN("DBHost:%s",            DBHost.c_str());
        LOG_RUN("DBUserName:%s",        DBUserName.c_str());
        LOG_RUN("DBPassword:%s",        DBPassword.c_str());
        LOG_RUN("DBName:%s",            DBName.c_str());
        LOG_RUN("ThreadNum:%d",         ThreadNum);
    }

public:
    int                 ProcID;             // 自身的procID
    std::string         IPCConfig;          // IPC的配置
    int                 GameServerProcID;   // GameServer的procID
    std::string         DBHost;             // DB的host url
    std::string         DBUserName;         // DB的用户名
    std::string         DBPassword;         // DB的密码
    std::string         DBName;             // DB的名字
    int                 ThreadNum;          // 线程数量
};

class DBProxyConfigFile
        : public ConfigFileSingleItem<DBProxyConfigItem>
        , public TMTSingleton<DBProxyConfigFile>
{
    // NULL
};

