/*
 * Bag.h
 *
 *  Created on: 2014年8月6日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/BagBehavior.h>
#include <Bag/BagItemKey.h>
#include <Bag/BagSlot.h>
#include <Engine/Util/Util.h>
#include <Protocol/Macro.pb.h>
#include <Util/EnumUtil.h>
#include <map>

class ComBagInfo;

class BagItem;
class Player;


class Bag
        : public Visualizable
        , public BagBehavior
{
public:
    Bag(Player& playerObj, BagType type);
    virtual             ~Bag();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

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

    // Bag
public:
    // 检测背包是否为空
    bool                Empty() const;

    // 获取背包物品种类数量
    UINT                GetBagItemKeyCount() const;

    bool                FillCSInfo(ComBagInfo& outBagInfo);
    bool                RestoreFromDB(const ComBagInfo& bagInfo);
    bool                StoreToDB(ComBagInfo& outBagInfo);

private:
    void                Clear();

    BagSlot*            GetBagSlotByBagItemKey(const BagItemKey& bagItemKey);
    void                ReleaseBagSlotByBagItemKey(const BagItemKey& bagItemKey);
    void                AddBagSlot(const BagSlot& slot);

private:
    typedef std::map<BagItemKey, BagSlot> BagItemKeyBagSlotMap;

private:
    BagType                 bagType;
    Player&                 player;
    BagItemKeyBagSlotMap    keyBagSlotMap;
};





