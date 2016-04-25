/*
 * BagItemCreatorManager.cpp
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#include <Bag/BagItemCreatorManager.h>
#include <Bag/BagItemKey.h>
#include <Engine/Log/LogMacro.h>
#include <stddef.h>
#include <utility>

using namespace std;

void BagItemCreatorManager::RegisterCreator(BagItemType itemType,
        BagItemCreator* creator)
{
    creatorMap[itemType] = creator;
}

BagItem* BagItemCreatorManager::CreateBagItem(const BagItemKey& key)
{
    BagItemTypeCreatorMap::iterator iter = creatorMap.find(key.Type);
    if (iter == creatorMap.end())
    {
        LOG_ERR("BagItemCreator<type:%s> is not found", BagItemType_Name(key.Type).c_str());
        return NULL;
    }

    return iter->second->CreateBagItem(key.ResID);
}
