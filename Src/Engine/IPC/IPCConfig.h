/*
 * IPCConfig.h
 *
 *  Created on: 2014年10月28日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Config/ConfigFile.h>
#include <Config/ConfigItem.h>
#include <string>
#include <vector>
#include <sstream>

/**
 * 通道配置
 */
class IPCChannelConfigItem : public ConfigItem
{
public:
    int                 Proc1;              // 进程1
    int                 Proc2;              // 进程2
    int                 Channel1to2Size;    // 通道1->2的大小
    int                 Channel2to1Size;    // 通道2->1的大小

public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_NUMBER(element, "Proc1",           Proc1);
        LOAD_XML_NUMBER(element, "Proc2",           Proc2);
        LOAD_XML_NUMBER(element, "Channel1to2Size", Channel1to2Size);
        LOAD_XML_NUMBER(element, "Channel2to1Size", Channel2to1Size);
        return true;
    }

public:
    bool                CheckHaveProc(int procID) const
    {
        return procID == Proc1 || procID == Proc2;
    }
};

/**
 * IPC配置文件
 */
class IPCConfigFile : public ConfigFile
{
public:
    typedef std::vector<IPCChannelConfigItem> ChannelConfigVector;

public:
    std::string         NamePrefix;     // 通道名称前缀
    ChannelConfigVector Channel;        // 通道

    // ConfigFile
public:
    virtual bool        LoadFromXml(const char* file, const char* itemParent = NULL)
    {
        if (!ConfigFile::LoadFromXml(file, itemParent))
        {
            return false;
        }

        TiXmlDocument configXml;
        if (!configXml.LoadFile(file))
        {
            LOG_ERR("Load xml <file:%s> fail: %s", file, configXml.ErrorDesc());
            return false;
        }

        const TiXmlElement* rootElement = configXml.RootElement();
        ASSERT_RETURN(rootElement != NULL, false);

        // 读取名称前缀
        LOAD_XML_STRING(rootElement, "NamePrefix", NamePrefix);

        // 读取通道配置
        const TiXmlElement* element = rootElement->FirstChildElement("Channel");
        while (element != NULL)
        {
            IPCChannelConfigItem configItem;

            // 解析。如果解析失败，跳过。
            if (!configItem.LoadFromXmlElement(element))
            {
                assert(false);
                element = element->NextSiblingElement();
                LOG_ERR("Load <conf:%s> <item:%s> fail", file, element->GetText());
                continue;
            }

            Channel.push_back(configItem);
            element = element->NextSiblingElement();
        }

        return true;
    }
};

struct IPCSubChannelType
{
    typedef enum
    {
        Proc1to2 = 0,
        Proc2to1 = 1,
    } Enum;
};

/**
 * IPC通道名称
 */
inline std::string IPCChannelName(const std::string& namePrefix,
                                  int index,
                                  const IPCChannelConfigItem& config,
                                  IPCSubChannelType::Enum subChannelType)
{
    std::stringstream nameStream;
    nameStream << namePrefix << '.' << index << '.';
    if (subChannelType == IPCSubChannelType::Proc1to2)
    {
        nameStream << config.Proc1 << "to" << config.Proc2;
    }
    else
    {
        nameStream << config.Proc2 << "to" << config.Proc1;
    }

    return nameStream.str();
}




