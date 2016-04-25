/*
 * PropConfig.h
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/Config/BagItemConfigItem.h>
#include <Engine/Language/EnumConverter.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/Config/ConfigItem.h>
#include <Engine/Config/ConfigMapIndexer.h>
#include <Engine/Config/TinyXmlUtil.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Engine/XML/tinyxml.h>
#include <Prop/PropDefine.h>
#include <cstdio>
#include <vector>


/**
 * 道具配置项
 */
class PropConfigItem : public ConfigItem
{
public:
    BagItemConfigItem   Item;
    PropType::Enum      Type;
    std::vector<int>    Param;

    // ConfigItem
public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        if (!Item.LoadFromXmlElement(element))
        {
            return false;
        }

        LOAD_XML_ATTR_VALUE(element, "Type", Type);
            ASSERT_RETURN(PropType::Converter::Valid(Type), false);
        LOAD_XML_ATTR_VALUE(element, "Param", Param);
        return true;
    }

    virtual UINT        GetID() const { return Item.ID; }
};


class PropConfigFile
        : public ConfigFileMultiItem<PropConfigItem>
        , public TMTSingleton<PropConfigFile>
{
public:
    PropConfigFile():Indexer(*this){}

    ConfigMapIndexer<PropConfigItem> Indexer;
};








