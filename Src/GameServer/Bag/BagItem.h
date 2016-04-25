/*
 * BagItem.h
 *
 *  Created on: 2014年8月6日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Util/GameObject.h>
#include <Engine/Language/Cloneable.h>
#include <Engine/Util/Util.h>
#include <Protocol/NetMsg.pb.h>

class ComBagItemInfo;
struct BagItemKey;

/**
 * 道具扩展接口
 */
class BagItemExtendable
{
public:
    virtual             ~BagItemExtendable(){}

    // 获取物品ResID
    virtual UINT        GetBagItemResID() const = 0;

    // 初始化道具
    virtual bool        Initialize(UINT resID) = 0;
};

/*
 * 可以放入背包中的物品
 */
class BagItem
        : public GameObject
        , public Cloneable<BagItem>
        , public BagItemExtendable
{
public:
    BagItem(BagItemType type);

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // BagItem
public:
    BagItemType         GetBagItemType() const;

    BagItemKey          GetBagItemKey() const;

    virtual bool        FillCSInfo(ComBagItemInfo& bagItemInfo) const;

    virtual bool        RestoreFromDB(const ComBagItemInfo& bagItemInfo);

private:
    BagItemType         bagItemType;
};




