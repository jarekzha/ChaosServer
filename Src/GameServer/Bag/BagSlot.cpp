/*
 * BagSlot.cpp
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief:
 */

#include <Bag/BagItem.h>
#include <Bag/BagItemCreatorManager.h>
#include <Bag/BagSlot.h>
#include <Bag/BagItemKey.h>
#include <Engine/Object/New.h>
#include <Engine/Util/Assert.h>
#include <Engine/Util/TMTSingleton.h>
#include <Protocol/NetMsg.pb.h>
#include <cassert>
#include <sstream>
#include <string>

using namespace std;

BagSlot::BagSlot()
{
    stackCount = 0;
    maxStackCount = 999;
    bagItem = NULL;
}

BagSlot::BagSlot(BagItem* bagItem, UINT count)
{
    this->stackCount = count;
    this->maxStackCount = 999;
    this->bagItem = bagItem;

    if (stackCount > maxStackCount)
    {
        stackCount = maxStackCount;
    }
}

BagSlot::~BagSlot()
{
    // NULL
}

string BagSlot::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "BagItem:" << bagItem->BriefInfo() << ", count:" << stackCount;
    return briefInfo.str();
}

UINT BagSlot::GetStackCount() const
{
    return stackCount;
}

int BagSlot::ChangeStackCount(int changeValue)
{
    int oldStackCount = stackCount;

    if (changeValue < 0 && (UINT)(-changeValue) > stackCount)
    {
        assert(false);
        stackCount = 0;
    }
    else
    {
        stackCount = stackCount + changeValue;
    }

    if (stackCount > maxStackCount)
    {
        stackCount = maxStackCount;
    }

    if (stackCount == 0)
    {
        Delete(bagItem);
        bagItem = NULL;
    }

    return (int)stackCount - oldStackCount;
}

void BagSlot::Clear()
{
    ChangeStackCount(GetStackCount() * -1);
}

bool BagSlot::Empty() const
{
    return GetStackCount() == 0 || bagItem == NULL;
}

BagItem* BagSlot::GetBagItem()
{
    return bagItem;
}

BagItem* BagSlot::TakeoutOneBagItem()
{
    ASSERT_RETURN(stackCount != 0 && bagItem != NULL, NULL);

    BagItem* newBagItem;

    if (stackCount == 1)
    {
        // 只有一个物品时，直接将次对象取出
        newBagItem = bagItem;
        bagItem = NULL;
    }
    else
    {
        // 有多个物品时，复制生成新的对象
        newBagItem = bagItem->Clone();
        ChangeStackCount(-1);
    }

    return newBagItem;
}

BagItemKey BagSlot::GetBagItemKey() const
{
    ASSERT_RETURN(bagItem != NULL, BagItemKey());

    return bagItem->GetBagItemKey();
}

bool BagSlot::FillCSInfo(ComBagSlotInfo& bagSlotInfo) const
{
    ASSERT_RETURN(!Empty(), false);

    bagSlotInfo.set_count(GetStackCount());
    return bagItem->FillCSInfo(*bagSlotInfo.mutable_bag_item());
}

bool BagSlot::RestoreFromDB(const ComBagSlotInfo& bagSlotInfo)
{
    ASSERT_RETURN(bagSlotInfo.count() != 0, false);

    bagItem = BagItemCreatorManager::Instance().CreateBagItem(BagItemKey(bagSlotInfo.bag_item()));
    ASSERT_RETURN(bagItem != NULL, false);

    if (!bagItem->RestoreFromDB(bagSlotInfo.bag_item()))
    {
        return false;
    }

    stackCount = bagSlotInfo.count();

    return true;
}

