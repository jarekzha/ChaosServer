/*
 * ConfigIndexer.h
 *
 *  Created on: 2014年8月14日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Config/ConfigParser.h>
#include <Config/ConfigFile.h>

/*
 * 配置索引器基类，继承他来实现具体索引器
 */
template <typename ConfigItemType>
class ConfigIndexer : public ConfigParser
{
public:
    typedef ConfigFileMultiItem<ConfigItemType> ConfigFileType;
    typedef typename ConfigFileMultiItem<ConfigItemType>::const_iterator ConfigFileConstIterator;

public:
    ConfigIndexer(ConfigFileMultiItem<ConfigItemType>& configFileMultiItem)
        : configFile(configFileMultiItem)
    {
        configFile.AddParser(*this);
    }

protected:
    const ConfigFileMultiItem<ConfigItemType>& ConfigFile() const
    {
        return configFile;
    }

private:
    ConfigFileMultiItem<ConfigItemType>& configFile;
};



