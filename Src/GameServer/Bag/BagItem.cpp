/*
 * BagItem.cpp
 *
 *  Created on: 2014年8月6日
 *  Author: jarek
 *  Brief: 
 */

#include <Bag/BagItem.h>
#include <Bag/BagItemKey.h>
#include <cstdio>
#include <string>

using namespace std;


BagItem::BagItem(BagItemType type)
{
    bagItemType = type;
}

BagItemType BagItem::GetBagItemType() const
{
    return bagItemType;
}

BagItemKey BagItem::GetBagItemKey() const
{
    return BagItemKey(bagItemType, GetBagItemResID());
}

std::string BagItem::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "itemType:" << BagItemType_Name(bagItemType)
            << ", resID:" << GetBagItemResID();

    return briefInfo.str();
}

bool BagItem::FillCSInfo(ComBagItemInfo& bagItemInfo) const
{
    bagItemInfo.set_bag_item_type(bagItemType);
    bagItemInfo.set_res_id(GetBagItemResID());
    return true;
}

bool BagItem::RestoreFromDB(const ComBagItemInfo& bagItemInfo)
{
    return true;
}
