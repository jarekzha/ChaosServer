/*
 * ConfigMultimapIndexer.h
 *
 *  Created on: 2014年8月15日
 *  Author: jarek
 *  Brief:
 */

#pragma once


#include <Config/ConfigIndexer.h>
#include <Config/ConfigFile.h>
#include <Config/ConfigItem.h>
#include <map>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

template <typename ConfigItemType>
UINT ConfigMultimapIndexerGetKey(const ConfigItemType& configItem)
{
    return configItem.GetID();
}

/*=============================================================================
 * 一对多的multimap索引
 */
template <typename ConfigItemType, typename Key = UINT, Key(*itemToKey)(const ConfigItemType&) = ConfigMultimapIndexerGetKey<ConfigItemType> >
class ConfigMultimapIndexer : public ConfigIndexer<ConfigItemType>
{
public:
    typedef typename std::multimap<Key, shared_ptr<const ConfigItemType> > ConfigItemMultimap;
    typedef typename std::multimap<Key, shared_ptr<const ConfigItemType> >::const_iterator const_iterator;

public:
    ConfigMultimapIndexer(ConfigFileMultiItem<ConfigItemType>& configFileMultiItem);
    virtual             ~ConfigMultimapIndexer(){}

    // ConfigParser
public:
    virtual bool        Parse();

    virtual void        Reset();

    // ConfigMultimapIndexer
public:
    const_iterator LowerBound(const Key& key) const;
    const_iterator UpperBound(const Key& key) const;

private:
    ConfigItemMultimap       configItemMultimap;
};

// Implementation
template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::ConfigMultimapIndexer(ConfigFileMultiItem<ConfigItemType>& configFileMultiItem)
    : ConfigIndexer<ConfigItemType>(configFileMultiItem)
{
    // NULL
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
bool ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::Parse()
{
    for (typename ConfigIndexer<ConfigItemType>::ConfigFileConstIterator iter = this->ConfigFile().All().begin();
            iter != this->ConfigFile().All().end(); ++iter)
    {
        shared_ptr<const ConfigItemType> configItem = *iter;
        Key key = itemToKey(*configItem);

        configItemMultimap.insert(typename ConfigItemMultimap::value_type(key, configItem));
    }

    return true;
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
void ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::Reset()
{
    configItemMultimap.clear();
}


template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
typename ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::const_iterator
    ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::LowerBound(const Key& key) const
{
    return configItemMultimap.lower_bound(key);
}

template <typename ConfigItemType, typename Key, Key(*itemToKey)(const ConfigItemType&)>
typename ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::const_iterator
    ConfigMultimapIndexer<ConfigItemType, Key, itemToKey>::UpperBound(const Key& key) const
{
    return configItemMultimap.upper_bound(key);
}

