/*
 * BagItemKey.cpp
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief:
 */


#include <Bag/BagItemKey.h>
#include <cstdio>
#include <string>

using namespace std;

BagItemKey::BagItemKey()
{
    Type = BAG_ITEM_TYPE_INVALID;
    ResID = 0;
}

BagItemKey::BagItemKey(BagItemType type, UINT resID)
{
    Type = type;
    ResID = resID;
}

BagItemKey::BagItemKey(const ComBagItemInfo& bagItem)
{
    Type = bagItem.bag_item_type();
    ResID = bagItem.res_id();
}

BagItemKey::BagItemKey(const ComBagItemKey& key)
{
    Type = key.item_type();
    ResID = key.item_res_id();
}

BagItemKey::operator ComBagItemKey() const
{
    ComBagItemKey key;
    key.set_item_type(Type);
    key.set_item_res_id(ResID);

    return key;
}

string BagItemKey::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "bagItemType:" << BagItemType_Name(Type)
            << ", resID:" << ResID;

    return briefInfo.str();
}

bool BagItemKey::IsInvalid() const
{
    return Type == BAG_ITEM_TYPE_INVALID || ResID == 0;
}

void BagItemKey::FillInfo(ComBagItemKey& key) const
{
    key.set_item_type(Type);
    key.set_item_res_id(ResID);
}

bool BagItemKey::operator ==(const BagItemKey& key) const
{
    return Type == key.Type && ResID == key.ResID;
}

bool BagItemKey::operator !=(const BagItemKey& key) const
{
    return !(*this == key);
}

bool BagItemKey::operator <(const BagItemKey& key) const
{
    if (Type < key.Type)
    {
        return true;
    }
    else if (Type == key.Type)
    {
        return ResID < key.ResID;
    }
    else
    {
        return false;
    }
}




