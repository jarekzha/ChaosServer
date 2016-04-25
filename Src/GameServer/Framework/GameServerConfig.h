/*
 * GameServerConfig.h
 *
 *  Created on: 2014年6月19日
 *      Author: jarek
 */

#pragma once
#include <string>
#include <Engine/Config/ConfigFile.h>
#include <Engine/Framework/LogicServerConfig.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>


/*
 * gameserer的配置项
 */
class GameServerConfigItem : public LogicServerConfigItem
{
public:
    int                 ConnectorProcID;                // Connector进程ID
    int                 GMConnectorProcID;              // GMConnector进错ID
    int                 DBProxyProcID;                  // DBProxy进程ID
    std::string         GameDataPath;                   // 游戏数据路径
    UINT                PlayerSaveIntervalSecond;       // 玩家存储间隔秒
    UINT                PlayerActiveSecond;             // 玩家活跃时间秒
    UINT                SessionActiveSecond;            // 会话活跃时间秒

    // ConfigItem
public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        if (!LogicServerConfigItem::LoadFromXmlElement(element))
        {
            return false;
        }

        LOAD_XML_VALUE(element,     "ConnectorProcID",          ConnectorProcID);
        LOAD_XML_VALUE(element,     "GMConnectorProcID",        GMConnectorProcID);
        LOAD_XML_VALUE(element,     "DBProxyProcID",            DBProxyProcID);
        LOAD_XML_STRING(element,    "GameDataPath",             GameDataPath);
        LOAD_XML_VALUE(element,     "PlayerSaveIntervalSecond", PlayerSaveIntervalSecond);
        LOAD_XML_VALUE(element,     "PlayerActiveSecond",       PlayerActiveSecond);
        LOAD_XML_VALUE(element,     "SessionActiveSecond",      SessionActiveSecond);

        return true;
    }

    // LogicServerConfigItem
public:
    virtual void            DisplayConfig() const
    {
        LogicServerConfigItem::DisplayConfig();
        LOG_RUN("ConnectorProcID:%d", ConnectorProcID);
        LOG_RUN("GMConnectorProcID:%d", GMConnectorProcID);
        LOG_RUN("DBProxyProcID:%d", DBProxyProcID);
        LOG_RUN("GameDataPath:%s", GameDataPath.c_str());
        LOG_RUN("PlayerSaveIntervalSecond:%d", PlayerSaveIntervalSecond);
        LOG_RUN("PlayerActiveSecond:%d", PlayerActiveSecond);
        LOG_RUN("SessionActiveSecond:%d", SessionActiveSecond);
    }
};

/*
 * GameServer配置文件
 */
class GameServerConfigFile
        : public ConfigFileSingleItem<GameServerConfigItem>
        , public TMTSingleton<GameServerConfigFile>
{
public:
    // null
};





