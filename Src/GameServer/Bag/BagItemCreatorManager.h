/*
 * BagItemCreatorManager.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/BagItemCreator.h>
#include <Engine/Util/TMTSingleton.h>
#include <Protocol/Macro.pb.h>
#include <map>

class BagItem;
class BagItemCreator;
struct BagItemKey;


class BagItemCreatorManager : public TMTSingleton<BagItemCreatorManager>
{
public:
    void                RegisterCreator(BagItemType itemType, BagItemCreator* creator);

    BagItem*            CreateBagItem(const BagItemKey& key);

private:
    typedef std::map<BagItemType, BagItemCreator*> BagItemTypeCreatorMap;

    BagItemTypeCreatorMap creatorMap;
};

