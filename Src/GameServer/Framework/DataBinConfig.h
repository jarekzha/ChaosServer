/*
 * DataBinConfig.h
 *
 *  Created on: 2014年8月11日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Config/ConfigFile.h>
#include <Engine/Config/ConfigItem.h>
#include <Engine/Config/TinyXmlUtil.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/XML/tinyxml.h>
#include <cstdio>
#include <map>
#include <string>


class DataBinConfigItem : public ConfigItem
{
public:
    std::string         Name;
    std::string         Url;

    // ConfigItem
public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_ATTR_STRING(element,   "Name",     Name);
        LOAD_XML_ATTR_STRING(element,   "Url",      Url);
        return true;
    }
};


class DataBinConfigFile
        : public ConfigFile
        , public TMTSingleton<DataBinConfigFile>
{
public:
    virtual             ~DataBinConfigFile(){}

    // ConfigFile
public:
    virtual bool        LoadFromXml(const char* file);

    // DataBinConfigFile
public:
    const std::string&  GetConfigUrl(const std::string& dataBinName);

    static const std::string Invalid;

private:
    typedef std::map<std::string, DataBinConfigItem*> NameDataBinMap;
    NameDataBinMap      nameDataBinMap;
};


