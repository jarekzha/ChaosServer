/*
 * PlayerBagSet.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/Bag.h>
#include <Bag/BagItemKey.h>
#include <Engine/Util/Util.h>
#include <Protocol/NetMsg.pb.h>
#include <Util/EnumUtil.h>
#include <map>


class Player;

class PlayerBagMap : public BagBehavior
{
public:
    PlayerBagMap(Player& playerObj);
    virtual             ~PlayerBagMap();

    // BagBehavior
public:
    virtual BagItem*    CreateAndAddBagItem(const BagItemKey& bagItemKey,
                                            UINT count,
                                            SyncOption sync = SyncOption_Yes);

    virtual bool        RemoveBagItem(const BagItemKey& bagItemKey,
                                      UINT count,
                                      SyncOption sync = SyncOption_Yes);

    virtual BagItem*    GetBagItem(const BagItemKey& bagItemKey);

    virtual BagItem*    TakeoutOneBagItem(const BagItemKey& bagItemKey,
                                          SyncOption sync = SyncOption_Yes);

    virtual UINT        GetBagItemCount(const BagItemKey& bagItemKey);

    // PlayerBagMap
public:
    bool                FillCSInfo(CSGetPlayerDataRsp& playerInfo);
    bool                RestoreFromDB(const SSPlayerDBInfo& playerInfo);
    bool                StoreToDB(SSPlayerDBInfo& playerDBInfo);

    BagType             GetBagTypeOfBagItemType(BagItemType bagItemType);

private:
    Bag*                GetBagByBagType(BagType bagType);
    Bag*                GetBagByItemType(BagItemType itemType);

private:
    typedef std::map<BagType, Bag*> BagTypeBagMap;
    typedef std::map<BagItemType, BagType> BagItemTypeBagTypeMap;

private:
    Player*                 player;
    BagTypeBagMap           bagMap;                     // 背包映射,背包类型->背包
    BagItemTypeBagTypeMap   bagItemTypeBagTypeMap;      // 背包物品映射，背包物品类型->背包类型
};



