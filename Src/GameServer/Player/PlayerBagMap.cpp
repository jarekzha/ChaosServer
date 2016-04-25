/*
 * PlayerBagSet.cpp
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief:
 */

#include <Bag/BagItem.h>
#include <Engine/Object/New.h>
#include <Player/Player.h>
#include <Player/PlayerBagMap.h>

PlayerBagMap::PlayerBagMap(Player& playerObj)
    : player(&playerObj)
{
    // 背包类型->背包
    bagMap[BAG_TYPE_HERO] = New<Bag>(playerObj, BAG_TYPE_HERO);
    bagMap[BAG_TYPE_COMMON] = New<Bag>(playerObj, BAG_TYPE_COMMON);
    bagMap[BAG_TYPE_FRAGMENT] = New<Bag>(playerObj, BAG_TYPE_FRAGMENT);
    bagMap[BAG_TYPE_ADVISER_SKILL] = New<Bag>(playerObj, BAG_TYPE_ADVISER_SKILL);

    // 背包物品类型->背包类型
    bagItemTypeBagTypeMap[BAG_ITEM_TYPE_HERO] = BAG_TYPE_HERO;
    bagItemTypeBagTypeMap[BAG_ITEM_TYPE_EQUIPMENT] = BAG_TYPE_COMMON;
    bagItemTypeBagTypeMap[BAG_ITEM_TYPE_PROP] = BAG_TYPE_COMMON;
    bagItemTypeBagTypeMap[BAG_ITEM_TYPE_FRAGMENT] = BAG_TYPE_FRAGMENT;
    bagItemTypeBagTypeMap[BAG_ITEM_TYPE_ADVISER_SKILL] = BAG_TYPE_ADVISER_SKILL;
}

PlayerBagMap::~PlayerBagMap()
{
    for (BagTypeBagMap::iterator iter = bagMap.begin(); iter != bagMap.end(); ++iter)
    {
        Delete(iter->second);
    }

    bagMap.clear();
}

BagItem* PlayerBagMap::CreateAndAddBagItem(const BagItemKey& bagItemKey,
                                           UINT count,
                                           SyncOption sync/* = SyncOption_Yes*/)
{
    Bag* bag = GetBagByItemType(bagItemKey.Type);
    ASSERT_RETURN(bag != NULL, NULL);

    return bag->CreateAndAddBagItem(bagItemKey, count, sync);
}

bool PlayerBagMap::RemoveBagItem(const BagItemKey& bagItemKey,
                                 UINT count,
                                 SyncOption sync/* = SyncOption_Yes*/)
{
    Bag* bag = GetBagByItemType(bagItemKey.Type);
    ASSERT_RETURN(bag != NULL, NULL);

    return bag->RemoveBagItem(bagItemKey, count, sync);
}

BagItem* PlayerBagMap::GetBagItem(const BagItemKey& bagItemKey)
{
    Bag* bag = GetBagByItemType(bagItemKey.Type);
    ASSERT_RETURN(bag != NULL, NULL);

    return bag->GetBagItem(bagItemKey);
}

BagItem* PlayerBagMap::TakeoutOneBagItem(const BagItemKey& bagItemKey,
                                         SyncOption sync/* = SyncOption_Yes*/)
{
    Bag* bag = GetBagByItemType(bagItemKey.Type);
    ASSERT_RETURN(bag != NULL, NULL);

    return bag->TakeoutOneBagItem(bagItemKey, sync);
}

UINT PlayerBagMap::GetBagItemCount(const BagItemKey& bagItemKey)
{
    Bag* bag = GetBagByItemType(bagItemKey.Type);
    ASSERT_RETURN(bag != NULL, NULL);

    return bag->GetBagItemCount(bagItemKey);
}

bool PlayerBagMap::FillCSInfo(CSGetPlayerDataRsp& playerInfo)
{
    bool res = true;
    res = res && bagMap[BAG_TYPE_HERO]->FillCSInfo(*playerInfo.mutable_hero_bag());
    res = res && bagMap[BAG_TYPE_COMMON]->FillCSInfo(*playerInfo.mutable_common_bag());
    res = res && bagMap[BAG_TYPE_FRAGMENT]->FillCSInfo(*playerInfo.mutable_fragment_bag());
    res = res && bagMap[BAG_TYPE_ADVISER_SKILL]->FillCSInfo(*playerInfo.mutable_adviser_skill_bag());
    return res;
}

bool PlayerBagMap::RestoreFromDB(const SSPlayerDBInfo& playerInfo)
{
    bool res = true;
    res = res && bagMap[BAG_TYPE_HERO]->RestoreFromDB(playerInfo.hero_bag());
    res = res && bagMap[BAG_TYPE_COMMON]->RestoreFromDB(playerInfo.common_bag());
    res = res && bagMap[BAG_TYPE_FRAGMENT]->RestoreFromDB(playerInfo.fragment_bag());
    res = res && bagMap[BAG_TYPE_ADVISER_SKILL]->RestoreFromDB(playerInfo.adviser_skill_bag());

    return res;
}

bool PlayerBagMap::StoreToDB(SSPlayerDBInfo& playerDBInfo)
{
    bool res = true;
    res = res && bagMap[BAG_TYPE_HERO]->StoreToDB(*playerDBInfo.mutable_hero_bag());
    res = res && bagMap[BAG_TYPE_COMMON]->StoreToDB(*playerDBInfo.mutable_common_bag());
    res = res && bagMap[BAG_TYPE_FRAGMENT]->StoreToDB(*playerDBInfo.mutable_fragment_bag());
    res = res && bagMap[BAG_TYPE_ADVISER_SKILL]->StoreToDB(*playerDBInfo.mutable_adviser_skill_bag());
    return res;
}

BagType PlayerBagMap::GetBagTypeOfBagItemType(BagItemType bagItemType)
{
    ASSERT_RETURN(BagItemType_IsValid(bagItemType) && bagItemType != BAG_ITEM_TYPE_INVALID, BAG_TYPE_INVALID);

    BagItemTypeBagTypeMap::iterator iter = bagItemTypeBagTypeMap.find(bagItemType);
    if (iter == bagItemTypeBagTypeMap.end())
    {
        assert(false);
        return BAG_TYPE_INVALID;
    }

    return iter->second;
}

Bag* PlayerBagMap::GetBagByBagType(BagType bagType)
{
    ASSERT_RETURN(BagType_IsValid(bagType) && bagType != BAG_TYPE_INVALID, NULL);

    BagTypeBagMap::iterator iter = bagMap.find(bagType);
    if (iter == bagMap.end())
    {
        assert(false);
        return NULL;
    }

    return iter->second;
}

Bag* PlayerBagMap::GetBagByItemType(BagItemType itemType)
{
    ASSERT_RETURN(BagItemType_IsValid(itemType) && itemType != BAG_ITEM_TYPE_INVALID, NULL);

    BagType bagType = GetBagTypeOfBagItemType(itemType);
    return GetBagByBagType(bagType);
}
