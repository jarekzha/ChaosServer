/*
 * BagSlot.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Language/Visualizable.h>
#include <Engine/Util/Util.h>
#include <string>

class ComBagSlotInfo;
struct BagItemKey;
class BagItem;

/*
 * 背包槽位
 */
class BagSlot : public Visualizable
{
public:
    BagSlot();
    BagSlot(BagItem* bagItem, UINT count);
    virtual             ~BagSlot();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // BagSlot
public:
    UINT                GetStackCount() const;

    // 改变叠加数量
    // return 真实改变值
    int                 ChangeStackCount(int changeValue);

    void                Clear();

    bool                Empty() const;

    // 获取一个物品，执行后，槽位中依然有此物品
    BagItem*            GetBagItem();

    // 取出一个物品，执行后，槽位中没有此物品
    BagItem*            TakeoutOneBagItem();

    BagItemKey          GetBagItemKey() const;

    bool                FillCSInfo(ComBagSlotInfo& bagSlotInfo) const;

    bool                RestoreFromDB(const ComBagSlotInfo& bagSlotInfo);

private:
    UINT                stackCount;
    UINT                maxStackCount;
    BagItem*            bagItem;
};


