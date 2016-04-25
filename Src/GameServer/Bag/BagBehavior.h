/*
 * BagBehavior.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Bag/BagItemKey.h>
#include <Engine/Util/Util.h>
#include <Util/EnumUtil.h>

class BagItem;

/**
 * 背包行为接口
 */
class BagBehavior
{
public:
    virtual             ~BagBehavior(){}

    // 创建和添加物品，
    // return 被创建的物品
    virtual BagItem*   CreateAndAddBagItem(const BagItemKey& bagItemKey,
                                           UINT count,
                                           SyncOption sync = SyncOption_Yes) = 0;

    // 删除指定物品
    // return 是否操作成功，如果指定物品不存在，或数量不够，会操作不成功。
    // 如果不成功，一个都不删除
    virtual bool        RemoveBagItem(const BagItemKey& bagItemKey,
                                      UINT count,
                                      SyncOption sync = SyncOption_Yes) = 0;

    // 获取指定物品，执行后背包中依然保存有此物品
    // return NULL 失败
    virtual BagItem*    GetBagItem(const BagItemKey& bagItemKey) = 0;

    // 取出一个指定物品，执行后背包中不再拥有此物品
    // return NULL表示失败
    virtual BagItem*    TakeoutOneBagItem(const BagItemKey& bagItemKey,
                                          SyncOption sync = SyncOption_Yes) = 0;

    // 获取制定物品数量
    virtual UINT        GetBagItemCount(const BagItemKey& bagItemKey) = 0;
};

