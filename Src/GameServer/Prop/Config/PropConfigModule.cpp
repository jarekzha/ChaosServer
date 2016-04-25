/*
 * PropConfigManager.cpp
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief:
 */

#include <Bag/Config/BagItemConfigModule.h>
#include <Bag/BagItemKey.h>
#include <Bag/Config/BagItemConfigItem.h>
#include <Engine/Config/ConfigFile.h>
#include <Engine/Framework/ModuleCenter.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Framework/GameServer.h>
#include <Prop/Config/PropConfig.h>
#include <Prop/Config/PropConfigModule.h>
#include <Protocol/Macro.pb.h>
#include <Util/GameDataUtil.h>
#include <list>

PropConfigModule::PropConfigModule()
{
    // NULL
}

PropConfigModule::~PropConfigModule()
{
    // NULL
}

bool PropConfigModule::Initialize()
{
    ASSERT_RETURN(GameDataFileMultiLineInitialize<PropConfigFile>("Prop"), false);

    AddItemConfig();
    return true;
}

void PropConfigModule::AddItemConfig()
{
    BagItemConfigModule& itemConfigModule = theModule(BagItemConfigModule);
    const PropConfigFile::ConfigItemList& itemList = PropConfigFile::Instance().All();
    for (PropConfigFile::const_iterator iter = itemList.begin();
            iter != itemList.end(); ++iter)
    {
        const PropConfigItem* config = iter->get();
        itemConfigModule.AddItemConfig(BagItemKey(BAG_ITEM_TYPE_PROP, config->Item.ID), config->Item);
    }
}
