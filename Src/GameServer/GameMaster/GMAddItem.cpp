/*
 * GMAddItem.cpp
 *
 *  Created on: 2014年10月20日
 *  Author: jarek
 *  Brief: 
 */

#include <GameMaster/GMAddItem.h>
#include <Engine/Log/LogMacro.h>
#include <Framework/GameServer.h>
#include <Player/PlayerModule.h>
#include <algorithm>

using namespace std;

GMAddItem::GMAddItem()
    :itemTypeMap()
{
    itemTypeMap.insert(ITEMTYPEMAP::value_type("hero", BAG_ITEM_TYPE_HERO));
    itemTypeMap.insert(ITEMTYPEMAP::value_type("equip", BAG_ITEM_TYPE_EQUIPMENT));
    itemTypeMap.insert(ITEMTYPEMAP::value_type("prop", BAG_ITEM_TYPE_PROP));
    itemTypeMap.insert(ITEMTYPEMAP::value_type("fragment", BAG_ITEM_TYPE_FRAGMENT));
}

GMAddItem::~GMAddItem()
{
    itemTypeMap.clear();
}

bool GMAddItem::HandleCommand(std::vector<std::string>& httpCommandParamVec)
{
    //additem name resType resId num
    if (httpCommandParamVec.size() < 5)
    {
        LOG_WARN("[GM-ADDITEM] param num[%u] invalid", (unsigned)httpCommandParamVec.size());
        return false;
    }

    std::string& playerName = httpCommandParamVec[1];
    std::string& itemTypeName = httpCommandParamVec[2];
    transform(itemTypeName.begin(), itemTypeName.end(), itemTypeName.begin(), ::tolower);

    ITEMTYPEMAP::iterator itemTypeNameMapIter = itemTypeMap.find(itemTypeName);
    if (itemTypeNameMapIter == itemTypeMap.end())
    {
        LOG_WARN("[GM-ADDITEM] itemType[%s] can't find itemtype", itemTypeName.c_str());
        return false;
    }

    BagItemType itemType = (BagItemType)itemTypeNameMapIter->second;

    int resId = atoi(httpCommandParamVec[3].c_str());
    int resNum = atoi(httpCommandParamVec[4].c_str());

    LOG_TRACE("[GM-ADDITEM] player[%s] resId[%d] resNum[%d]", playerName.c_str(), resId, resNum);

    PlayerModule& playerModule = theModule(PlayerModule);
    Player* player = playerModule.GetPlayerByAccount(playerName);
    if (NULL == player)
    {
        LOG_WARN("[GM-ADDITEM] can't find player[%s]", playerName.c_str());
        return false;
    }

    BagItemKey bagItemKey;
    bagItemKey.Type = itemType;
    bagItemKey.ResID = resId;

    PlayerBagMap& playerBag = player->GetBagMap();
    BagItem* bagItgem = playerBag.CreateAndAddBagItem(bagItemKey, resNum);
    if (NULL != bagItgem)
    {
        LOG_RUN("[GM-ADDITEM] player[%s] add[%s] num[%d] success",
                playerName.c_str(), itemTypeName.c_str(), resNum);

        //to-do
    }

    return true;
}




