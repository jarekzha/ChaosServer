/*
 * Bag.cpp
 *
 *  Created on: 2014年8月6日
 *  Author: jarek
 *  Brief:
 */

#include <Bag/Bag.h>
#include <Bag/BagItem.h>
#include <Bag/BagItemCreatorManager.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>
#include <Message/Message/CSResponse.h>
#include <Player/Player.h>
#include <Protocol/NetMsg.pb.h>
#include <Protocol/MsgID.pb.h>
#include <cassert>
#include <cstdio>
#include <string>
#include <utility>

struct BagItemKey;

using namespace std;


Bag::Bag(Player& playerObj, BagType type)
    : player(playerObj)
{
    bagType = type;
}

Bag::~Bag()
{
    Clear();
}

string Bag::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "bagType:" << BagType_Name(bagType)
            <<  ", bagItemKeyCount:" << GetBagItemKeyCount();

    return briefInfo.str();
}

bool Bag::Empty() const
{
    return GetBagItemKeyCount() == 0;
}

UINT Bag::GetBagItemKeyCount() const
{
    return keyBagSlotMap.size();
}

BagItem* Bag::CreateAndAddBagItem(const BagItemKey& bagItemKey, UINT count,
        SyncOption sync/* = SyncOption_Yes*/)
{
    BagItem* newBagItem = NULL;

    // 如果已有相同item，直接叠加
    BagSlot* slot = GetBagSlotByBagItemKey(bagItemKey);
    if (slot != NULL)
    {
        newBagItem = slot->GetBagItem();
        count = slot->ChangeStackCount(count);
    }
    else
    {
        BagItemCreatorManager& creatorMgr = BagItemCreatorManager::Instance();
        newBagItem = creatorMgr.CreateBagItem(bagItemKey);
        if (NULL == newBagItem)
        {
            LOG_ERR("Create Bag item with key<%s> fail", bagItemKey.BriefInfo().c_str());
            if (SyncOption_Yes == sync)
            {
                CSSubResponse::SendErrorMsg(player, MSG_ERROR_INVALID_DATA);
            }
            return NULL;
        }

        assert(newBagItem->GetBagItemKey() == bagItemKey);
        BagSlot slot = BagSlot(newBagItem, count);
        count = slot.GetStackCount();
        AddBagSlot(slot);
    }

    if (SyncOption_Yes == sync)
    {
        CSSubResponse response(player);
        NetMsg& rspMsg = response.GetMsg();
        rspMsg.set_id(CS_MSG_BAG_ITEM_ADD_NTF);
        CSBagItemAddNtf& bagItemAddNtf = *rspMsg.mutable_body()->mutable_cs_bag_item_add_ntf();

        bagItemAddNtf.set_bag_type(bagType);
        newBagItem->FillCSInfo(*bagItemAddNtf.mutable_bag_item());
        bagItemAddNtf.set_add_count(count);

        response.Send();
    }

    LOG_TRACE("Player<%s> add bagItem<%s, count:%u>, then bag<%s>",
            player.BriefInfo().c_str(), newBagItem->BriefInfo().c_str(), count, BriefInfo().c_str());

    return newBagItem;
}

bool Bag::RemoveBagItem(const BagItemKey& bagItemKey, UINT count, SyncOption sync/* = SyncOption_Yes*/)
{
    BagSlot* slot = GetBagSlotByBagItemKey(bagItemKey);
    if (NULL == slot)
    {
        return false;
    }

    if (slot->GetStackCount() < count)
    {
        return false;
    }

    BagItem* bagItem = slot->GetBagItem();
    ASSERT_RETURN(bagItem != NULL, false);

    string bagItemBriefInfo = bagItem->BriefInfo().c_str();

    slot->ChangeStackCount(count * -1);
    if (slot->Empty())
    {
        ReleaseBagSlotByBagItemKey(bagItemKey);
    }

    if (SyncOption_Yes == sync)
    {
        CSSubResponse response(player);
        NetMsg& rspMsg = response.GetMsg();
        rspMsg.set_id(CS_MSG_BAG_ITEM_REMOVE_NTF);
        CSBagItemRemoveNtf& bagItemRemoveNtf = *rspMsg.mutable_body()->mutable_cs_bag_item_remove_ntf();

        bagItemRemoveNtf.set_bag_type(bagType);
        *bagItemRemoveNtf.mutable_item_key() = bagItemKey;
        bagItemRemoveNtf.set_remove_count(count);

        response.Send();
    }

    LOG_TRACE("Player<%s> remove bagItem<%s, count:%u>, then bag<%s>",
            player.BriefInfo().c_str(), bagItemBriefInfo.c_str(), count, BriefInfo().c_str());

    return true;
}

BagItem* Bag::GetBagItem(const BagItemKey& bagItemKey)
{
    BagSlot* slot = GetBagSlotByBagItemKey(bagItemKey);
    if (NULL == slot)
    {
        return NULL;
    }

    ASSERT_RETURN(slot->GetStackCount() != 0, NULL);
    return slot->GetBagItem();
}

BagItem* Bag::TakeoutOneBagItem(const BagItemKey& bagItemKey, SyncOption sync/* = SyncOption_Yes*/)
{
    BagSlot* slot = GetBagSlotByBagItemKey(bagItemKey);
    if (NULL == slot)
    {
        return NULL;
    }

    BagItem* newBagItem = slot->TakeoutOneBagItem();
    if (slot->Empty())
    {
        ReleaseBagSlotByBagItemKey(bagItemKey);
    }

    if (SyncOption_Yes == sync)
    {
        CSSubResponse response(player);
        NetMsg& rspMsg = response.GetMsg();
        rspMsg.set_id(CS_MSG_BAG_ITEM_REMOVE_NTF);
        CSBagItemRemoveNtf& bagItemRemoveNtf = *rspMsg.mutable_body()->mutable_cs_bag_item_remove_ntf();

        bagItemRemoveNtf.set_bag_type(bagType);
        *bagItemRemoveNtf.mutable_item_key() = bagItemKey;
        bagItemRemoveNtf.set_remove_count(1);

        response.Send();
    }

    LOG_TRACE("Player<%s> takeout bagItem<%s, count:%d>, then bag<%s>",
            player.BriefInfo().c_str(), newBagItem->BriefInfo().c_str(), 1, BriefInfo().c_str());

    return newBagItem;
}

UINT Bag::GetBagItemCount(const BagItemKey& bagItemKey)
{
    BagSlot* slot = GetBagSlotByBagItemKey(bagItemKey);
    if (NULL == slot)
    {
        return 0;
    }

    return slot->GetStackCount();
}

bool Bag::FillCSInfo(ComBagInfo& outBagInfo)
{
    outBagInfo.set_bag_type(bagType);

    for (BagItemKeyBagSlotMap::iterator iter = keyBagSlotMap.begin(); iter != keyBagSlotMap.end(); ++iter)
    {
        BagSlot& slot = iter->second;
        ComBagSlotInfo* bagSlotInfo = outBagInfo.mutable_bag_slot()->Add();

        if (!slot.FillCSInfo(*bagSlotInfo))
        {
            outBagInfo.mutable_bag_slot()->RemoveLast();
            return false;
        }
    }

    return true;
}

bool Bag::RestoreFromDB(const ComBagInfo& bagInfo)
{
    Clear();

    for (int i = 0; i < bagInfo.bag_slot_size(); ++i)
    {
        const ComBagSlotInfo& comBagSlot = bagInfo.bag_slot(i);
        BagSlot slot;
        if (!slot.RestoreFromDB(comBagSlot))
        {
            return false;
        }

        AddBagSlot(slot);
    }

    return true;
}

bool Bag::StoreToDB(ComBagInfo& outBagInfo)
{
    return FillCSInfo(outBagInfo);
}

void Bag::Clear()
{
    for (BagItemKeyBagSlotMap::iterator iter = keyBagSlotMap.begin();
            iter != keyBagSlotMap.end(); ++iter)
    {
        iter->second.Clear();
    }

    keyBagSlotMap.clear();
}

BagSlot* Bag::GetBagSlotByBagItemKey(const BagItemKey& bagItemKey)
{
    BagItemKeyBagSlotMap::iterator iter = keyBagSlotMap.find(bagItemKey);
    if (iter == keyBagSlotMap.end())
    {
        return NULL;
    }

    return &iter->second;
}

void Bag::ReleaseBagSlotByBagItemKey(const BagItemKey& bagItemKey)
{
    BagItemKeyBagSlotMap::iterator iter = keyBagSlotMap.find(bagItemKey);
    if (iter == keyBagSlotMap.end())
    {
        return;
    }

    keyBagSlotMap.erase(iter);
}

void Bag::AddBagSlot(const BagSlot& slot)
{
    BagItemKey bagItemKey = slot.GetBagItemKey();
    BagItemKeyBagSlotMap::iterator iter = keyBagSlotMap.find(bagItemKey);
    if (iter != keyBagSlotMap.end())
    {
        LOG_ERR("Player<%s> add slot<%s> into bag<%s> fail: key<%s> exist",
                player.BriefInfo().c_str(), slot.BriefInfo().c_str(), this->BriefInfo().c_str(), bagItemKey.BriefInfo().c_str());
        assert(false);
        return;
    }

    keyBagSlotMap.insert(BagItemKeyBagSlotMap::value_type(bagItemKey, slot));
}

