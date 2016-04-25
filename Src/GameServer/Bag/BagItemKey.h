/*
 * BagItemKey.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Language/Visualizable.h>
#include <Engine/Util/Util.h>
#include <Protocol/NetMsg.pb.h>

class ComBagItemInfo;

/**
 * 物品类型key
 */
struct BagItemKey : public Visualizable
{
    BagItemType         Type;
    UINT                ResID;

public:
    BagItemKey();
    BagItemKey(BagItemType type, UINT resID);
    BagItemKey(const ComBagItemInfo& bagItem);
    BagItemKey(const ComBagItemKey& key);

    operator ComBagItemKey() const;

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // BagItemKey
public:
    bool                IsInvalid() const;

    void                FillInfo(ComBagItemKey& key) const;

    bool operator==(const BagItemKey& key) const;
    bool operator!=(const BagItemKey& key) const;
    bool operator<(const BagItemKey& key) const;
};

