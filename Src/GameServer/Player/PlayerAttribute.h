/*
 * PlayerAttribute.h
 *
 *  Created on: 2014年8月25日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/Util.h>
#include <Protocol/Macro.pb.h>
#include <Util/EnumUtil.h>
#include <string>

class CSPlayerBaseInfo;
class SSPlayerBaseDBInfo;
class Player;

/**
 * 角色基础属性
 */
class PlayerAttribute
{
public:
    PlayerAttribute(Player& playerObj);
    virtual             ~PlayerAttribute();

    // PlayerAttribute
public:
    void                SetName(const std::string& name);
    const std::string&  GetName() const;

    UINT                GetAttr(PlayerAttrType attrType) const;
    void                ChangeAttr(PlayerAttrType attrType, int changeValue,
                                SyncOption sync = SyncOption_Yes);

    bool                FillCSInfo(CSPlayerBaseInfo& playerBaseInfo) const;
    bool                RestoreFromDB(const SSPlayerBaseDBInfo& playerBaseDBInfo);
    bool                StoreToDB(SSPlayerBaseDBInfo& playerBaseDBInfo) const;

private:
    Player&             player;

    std::string         name;
    UINT                attribute[PlayerAttrType_ARRAYSIZE];
};



