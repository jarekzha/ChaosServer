/*
 * RobotConfig.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Config/ConfigFile.h>
#include <Engine/Config/ConfigItem.h>
#include <Engine/Config/TinyXmlUtil.h>
#include <Engine/Util/Util.h>
#include <Engine/XML/tinyxml.h>
#include <string>


class RobotConfigItem : public ConfigItem
{
public:
    std::string         ServerHost;     // 服务器host url
    WORD                ServerPort;     // 服务器端口
    UINT                RobotNum;       // 开启机器人数量

public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_STRING(element,    "ServerHost",   ServerHost);
        LOAD_XML_VALUE(element,     "ServerPort",   ServerPort);
        LOAD_XML_VALUE(element,     "RobotNum",     RobotNum);
        return true;
    }
};


class RobotConfigFile : public ConfigFileSingleItem<RobotConfigItem>
{
    // NULL
};

