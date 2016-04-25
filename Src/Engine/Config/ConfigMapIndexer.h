/*
 * ConfigMapIndexer.h
 *
 *  Created on: 2014年8月14日
 *  Author: jarek
 *  Brief: 
 */

#pragma once


#include <Config/ConfigIndexer.h>
#include <Config/ConfigFile.h>
#include <Config/ConfigItem.h>
#include <boost/shared_ptr.hpp>
#include <map>

using boost::shared_ptr;

template <typename ConfigItemType>
UINT ConfigMapIndexerGetKey(const ConfigItemType& configItem)
{
    return configItem.GetID();
}

/*=============================================================================
 * 一对一的map索引
 */
template <typename ConfigItemType, typename Key = UINT, Key(*itemToKey)(const ConfigItemType&) = ConfigMapIndexerGetKey<ConfigItemType> >
class ConfigMapIndexer : public ConfigIndexer<ConfigItemType>
{
public:
    ConfigMapIndexer(ConfigFileMultiItem<ConfigItemType>& configFileMultiItem);
    virtual             ~ConfigMapIndexer(){}

    // ConfigParser
public:
    virtual bool        Parse();

    virtual void        Reset();

    // ConfigMapIndexer
public:
    // 通过key来获取配置指针
    const ConfigItemType* operator[](const Key& key) const;

    // 通过key来获取配置指针
    const ConfigItemType* GetItem(const Key& key) const;

    // 通过key来获取配置指针，如果要热更新配置，只有通过此接口获取的指针可以长时间持有
    shared_ptr<const ConfigItemType> GetItemToHold(const Key& key) const;

private:
    typedef typename std::map<Key, shared_ptr<const ConfigItemType> > ConfigItemMap;

private:
    ConfigItemMap       configItemMap;
};

// Implementation
template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
bool ConfigMapIndexer<ConfigItemType, Key, itemToKey>::Parse()
{
    for (typename ConfigIndexer<ConfigItemType>::ConfigFileConstIterator iter = this->ConfigFile().All().begin();
            iter != this->ConfigFile().All().end(); ++iter)
    {
        shared_ptr<const ConfigItemType> itemPtr = *iter;
        Key key = itemToKey(*itemPtr);
        if (GetItem(key) != NULL)
        {
            assert(false);
            return false;
        }

        configItemMap.insert(typename ConfigItemMap::value_type(key, itemPtr));
    }

    return true;
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
void ConfigMapIndexer<ConfigItemType, Key, itemToKey>::Reset()
{
    configItemMap.clear();
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
ConfigMapIndexer<ConfigItemType, Key, itemToKey>::ConfigMapIndexer(ConfigFileMultiItem<ConfigItemType>& configFileMultiItem)
    : ConfigIndexer<ConfigItemType>(configFileMultiItem)
{
    // NULL
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
const ConfigItemType* ConfigMapIndexer<ConfigItemType, Key, itemToKey>::operator[](const Key& key) const
{
    return GetItem(key);
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
const ConfigItemType* ConfigMapIndexer<ConfigItemType, Key, itemToKey>::GetItem(const Key& key) const
{
    typename ConfigItemMap::const_iterator iter = configItemMap.find(key);
    if (iter == configItemMap.end())
    {
        return NULL;
    }

    return iter->second.get();
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
shared_ptr<const ConfigItemType> ConfigMapIndexer<ConfigItemType, Key, itemToKey>::GetItemToHold(const Key& key) const
{
    typename ConfigItemMap::const_iterator iter = configItemMap.find(key);
    if (iter == configItemMap.end())
    {
        shared_ptr<const ConfigItemType> nullPtr;
        return nullPtr;
    }

    return iter->second;
}


