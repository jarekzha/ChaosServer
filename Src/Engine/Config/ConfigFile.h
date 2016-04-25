/*
 * ConfigFile.h
 *
 *  Created on: 2014年6月18日
 *      Author: jarekzhang
 */

#pragma once

#include <libgen.h>
#include <list>
#include <Log/LogMacro.h>
#include <Language/String.h>
#include <Util/Util.h>
#include <XML/tinyxml.h>
#include <Config/TinyXmlUtil.h>
#include <Config/ConfigParser.h>
#include <Config/ConfigItem.h>
#include <boost/shared_ptr.hpp>
#include <string.h>

using boost::shared_ptr;

/*
 * 配置文件基础类
 */
class ConfigFile
{
public:
    virtual             ~ConfigFile(){}

    // 读取指定xml文件的指定item，通过"xx.xx.xx"的方式指定节点，默认为根节点
    virtual bool        LoadFromXml(const char* file, const char* itemParent = NULL)
    {
        ASSERT_RETURN(file != NULL, false);

        char tempFilePath[PATH_MAX];
        tempFilePath[0] = 0;

        fileFullPath = file;

        StrCpy(tempFilePath, file, PATH_MAX);
        fileDir = dirname(tempFilePath);
        fileDir += '/';

        StrCpy(tempFilePath, file, PATH_MAX);
        fileName = basename(tempFilePath);

        return true;
    }

public:
    const std::string&  GetFileFullPath() const  { return fileFullPath; }
    const std::string&  GetFileDir() const       { return fileDir; }
    const std::string&  GetFileName() const      { return fileName; }

private:
    std::string         fileFullPath;   // 文件完整路径包括文件名
    std::string         fileDir;        // 文件所在路径
    std::string         fileName;       // 文件名称
};

/*=============================================================================
 * 单个配置文件，其中会包含多个配置项
 */
template <typename ConfigItemType>
class ConfigFileMultiItem : public ConfigFile
{
public:
    typedef shared_ptr<const ConfigItemType> ItemPtrType;
    typedef typename std::list<ItemPtrType> ConfigItemList;
    typedef typename ConfigItemList::const_iterator const_iterator;

public:
    ConfigFileMultiItem();
    virtual             ~ConfigFileMultiItem();

    // ConfigFile
public:
    virtual bool        LoadFromXml(const char* file, const char* itemParent = NULL);

    // ConfigFileMultiItem
public:
    const ConfigItemList& All() const;

    void                AddParser(ConfigParser& parser);

private:
    typedef std::list<ConfigParser*> ParserList;

private:
    ConfigItemList      configItemList;
    ParserList          parserList;
    bool                loaded;
};

// Implementation
template <typename ConfigItemType>
ConfigFileMultiItem<ConfigItemType>::ConfigFileMultiItem()
: loaded(false)
{
    // NULL
}

template <typename ConfigItemType>
ConfigFileMultiItem<ConfigItemType>::~ConfigFileMultiItem()
{
    configItemList.clear();
}

template <typename ConfigItemType>
bool ConfigFileMultiItem<ConfigItemType>::LoadFromXml(const char* file, const char* itemParent/*= NULL*/)
{
    ASSERT_RETURN(file != NULL, false);

    // 此前加载过，先清空此前加载的结果
    if (loaded)
    {
        configItemList.clear();
        for (ParserList::iterator iter = parserList.begin(); iter != parserList.end(); ++iter)
        {
            (*iter)->Reset();
        }

        loaded = false;
    }

    if (!ConfigFile::LoadFromXml(file))
    {
        return false;
    }

    TiXmlDocument configXml;
    if (!configXml.LoadFile(file))
    {
        LOG_ERR("Load xml <file:%s> fail: %s", file, configXml.ErrorDesc());
        return false;
    }

    TiXmlElement* parentElement = NULL;
    if (itemParent != NULL)
    {
        parentElement = FindFirstElement(configXml, itemParent);
        if (NULL == parentElement)
        {
            LOG_ERR("Get xml <file:%s> element<%s> fail", file, itemParent);
            return false;
        }
    }
    else
    {
        parentElement = configXml.RootElement();
        if (NULL == parentElement)
        {
            LOG_ERR("Get xml <file:%s> root element failed! error:%s", file, configXml.ErrorDesc());
            return false;
        }
    }

    TiXmlElement* element = parentElement->FirstChildElement();
    while (element != NULL)
    {
        ConfigItemType* configItem = new ConfigItemType;
        ASSERT_RETURN(configItem != NULL, false);

        // 解析。如果解析失败，跳过。
        if (!configItem->LoadFromXmlElement(element))
        {
            assert(false);
            element = element->NextSiblingElement();
            delete configItem;
            LOG_ERR("Load <conf:%s> <item:%s> fail", file, element->GetText());
            continue;
        }

        configItemList.push_back(ItemPtrType(configItem));
        element = element->NextSiblingElement();
    }

    for (ParserList::iterator iter = parserList.begin(); iter != parserList.end(); ++iter)
    {
        if (!(*iter)->Parse())
        {
            LOG_ERR("Config<path:%s> parser parse fail", file);
            assert(false);
            return false;
        }
    }

    loaded = true;

    return true;
}

template <typename ConfigItemType>
const typename ConfigFileMultiItem<ConfigItemType>::ConfigItemList&
    ConfigFileMultiItem<ConfigItemType>::All() const
{
    return configItemList;
}

template <typename ConfigItemType>
void ConfigFileMultiItem<ConfigItemType>::AddParser(ConfigParser& parser)
{
    parserList.push_back(&parser);
}

/*=============================================================================
 * 单个配置文件，其中只有一行配置项
 */
template <typename ConfigItemType>
class ConfigFileSingleItem : public ConfigFile
{
public:
    ConfigFileSingleItem();
    virtual             ~ConfigFileSingleItem(){}

    // ConfigFile
public:
    virtual bool        LoadFromXml(const char* file, const char* itemParent = NULL);

    // ConfigFileSingleItem
public:
    const ConfigItemType& Item() const;

private:
    ConfigItemType      item;
    bool                loaded;
};

template <typename ConfigItemType>
ConfigFileSingleItem<ConfigItemType>::ConfigFileSingleItem()
: loaded(false)
{
    // NULL
}

template <typename ConfigItemType>
bool ConfigFileSingleItem<ConfigItemType>::LoadFromXml(const char* file, const char* itemParent/* = NULL*/)
{
    ASSERT_RETURN(file != NULL, false);

    if (loaded)
    {
        // 重复加载时，先调用析构函数，再调用构造函数，以此来清空此前加载的数据
        ((ConfigItem*)&item)->~ConfigItem();
        new (&item) ConfigItemType();
    }

    if (!ConfigFile::LoadFromXml(file))
    {
        return false;
    }

    TiXmlDocument configXml;
    if (!configXml.LoadFile(file))
    {
        LOG_ERR("Load xml <file:%s> fail: %s", file, configXml.ErrorDesc());
        return false;
    }

    TiXmlElement* parentElement = NULL;
    if (itemParent != NULL)
    {
        parentElement = FindFirstElement(configXml, itemParent);
        if (NULL == parentElement)
        {
            LOG_ERR("Get xml <file:%s> element<%s> fail", file, itemParent);
            return false;
        }
    }
    else
    {
        parentElement = configXml.RootElement();
        if (NULL == parentElement)
        {
            LOG_ERR("Get xml <file:%s> root element failed! error:%s", file, configXml.ErrorDesc());
            return false;
        }
    }

    if (!item.LoadFromXmlElement(parentElement))
    {
        assert(false);
        LOG_ERR("Load <conf:%s> <item:%s> fail", file, parentElement->GetText());
        return false;
    }

    loaded = true;
    return true;
}

template <typename ConfigItemType>
const ConfigItemType& ConfigFileSingleItem<ConfigItemType>::Item() const
{
    return item;
}

