
/*
 * PlayerAttribute.cpp
 *
 *  Created on: 2014年8月25日
 *  Author: jarek
 *  Brief:
 */



#include <Message/Message/CSResponse.h>
#include <Player/PlayerAttribute.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/MsgID.pb.h>
#include <Util/EnumUtil.h>
#include <Engine/Util/Util.h>

PlayerAttribute::PlayerAttribute(Player& playerObj)
    : player(playerObj)
{
    memset(attribute, 0, sizeof(attribute));

    attribute[PLAYER_ATTR_TYPE_LEVEL] = 1;
}

PlayerAttribute::~PlayerAttribute()
{
}

void PlayerAttribute::SetName(const std::string& name)
{
    this->name = name;
}

const std::string& PlayerAttribute::GetName() const
{
    return name;
}

UINT PlayerAttribute::GetAttr(PlayerAttrType attrType) const
{
    ASSERT_RETURN(PlayerAttrType_IsValid(attrType), 0);

    return attribute[attrType];
}

void PlayerAttribute::ChangeAttr(PlayerAttrType attrType, int changeValue, SyncOption sync/* = SyncOption_Yes*/)
{
    ASSERT_RETURN_VOID(PlayerAttrType_IsValid(attrType));

    if (changeValue < 0 && (UINT)(-changeValue) > attribute[attrType])
    {
        changeValue = attribute[attrType] * -1;
    }

    if (0 == changeValue)
    {
        return;
    }

    attribute[attrType] += changeValue;

    if (sync == SyncOption_Yes)
    {
        CSSubResponse response(player);
        Msg& rspMsg = response.GetMsg();

        rspMsg.set_id(CS_MSG_PLAYER_ATTR_CHANGE_NTF);
        CSPlayerAttrChangeNtf& attrChgNtf = *rspMsg.mutable_body()->mutable_cs_player_attr_change_ntf();
        attrChgNtf.set_attr_type(attrType);
        attrChgNtf.set_attr_value(attribute[attrType]);

        response.Send();
    }
}

bool PlayerAttribute::FillCSInfo(CSPlayerBaseInfo& playerBaseInfo) const
{
    playerBaseInfo.set_name(name);
    playerBaseInfo.set_game_money(attribute[PLAYER_ATTR_TYPE_GAME_MONEY]);
    playerBaseInfo.set_rmb(attribute[PLAYER_ATTR_TYPE_RMB]);
    playerBaseInfo.set_cur_energy(attribute[PLAYER_ATTR_TYPE_CUR_ENERGY]);
    playerBaseInfo.set_max_energy(attribute[PLAYER_ATTR_TYPE_MAX_ENERGY]);
    playerBaseInfo.set_exp(attribute[PLAYER_ATTR_TYPE_EXP]);
    playerBaseInfo.set_level(attribute[PLAYER_ATTR_TYPE_LEVEL]);

    return true;
}

bool PlayerAttribute::RestoreFromDB(const SSPlayerBaseDBInfo& playerBaseDBInfo)
{
    name = playerBaseDBInfo.name();
    attribute[PLAYER_ATTR_TYPE_GAME_MONEY] = playerBaseDBInfo.game_money();
    attribute[PLAYER_ATTR_TYPE_RMB] = playerBaseDBInfo.rmb();
    attribute[PLAYER_ATTR_TYPE_CUR_ENERGY] = playerBaseDBInfo.cur_energy();
    attribute[PLAYER_ATTR_TYPE_MAX_ENERGY] = playerBaseDBInfo.max_energy();
    attribute[PLAYER_ATTR_TYPE_EXP] = playerBaseDBInfo.exp();
    attribute[PLAYER_ATTR_TYPE_LEVEL] = playerBaseDBInfo.level();

    return true;
}

bool PlayerAttribute::StoreToDB(SSPlayerBaseDBInfo& playerBaseDBInfo) const
{
    playerBaseDBInfo.set_name(name);
    playerBaseDBInfo.set_game_money(attribute[PLAYER_ATTR_TYPE_GAME_MONEY]);
    playerBaseDBInfo.set_rmb(attribute[PLAYER_ATTR_TYPE_RMB]);
    playerBaseDBInfo.set_cur_energy(attribute[PLAYER_ATTR_TYPE_CUR_ENERGY]);
    playerBaseDBInfo.set_max_energy(attribute[PLAYER_ATTR_TYPE_MAX_ENERGY]);
    playerBaseDBInfo.set_exp(attribute[PLAYER_ATTR_TYPE_EXP]);
    playerBaseDBInfo.set_level(attribute[PLAYER_ATTR_TYPE_LEVEL]);

    return true;
}
