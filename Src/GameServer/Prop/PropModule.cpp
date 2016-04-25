/*
 * PropModule.cpp
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief:
 */

#include <Bag/BagItemCreatorManager.h>
#include <Common/Factory/Factory.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>
#include <Engine/Util/Util.h>
#include <Prop/Config/PropConfig.h>
#include <Prop/Prop/PropFactory.h>
#include <Prop/Prop/Prop.h>
#include <Prop/PropModule.h>
#include <Protocol/Macro.pb.h>
#include <cstdio>


PropModule::PropModule()
{
    // NULL
}

PropModule::~PropModule()
{
    // NULL
}

bool PropModule::Initialize()
{
    BagItemCreatorManager::Instance().RegisterCreator(BAG_ITEM_TYPE_PROP, this);
    return true;
}

BagItem* PropModule::CreateBagItem(UINT resID)
{
    ASSERT_RETURN(resID != 0, NULL);

    const PropConfigItem* propConfig =  PropConfigFile::Instance().Indexer[resID];
    if (NULL == propConfig)
    {
        LOG_ERR("Create Prop<resID:%u> fail, no config found", resID);
        return NULL;
    }

    Prop* newProp = PropFactory::Instance().Create(propConfig->Type);
    if (NULL == newProp)
    {
        LOG_ERR("Create Prop<resID:%u, type:%d> fail, invalid type",
                propConfig->Item.ID, propConfig->Type);
        return NULL;
    }

    if (!newProp->Initialize(resID))
    {
        LOG_ERR("Create Prop<resID:%u, type:%d> fail, initialize fail",
                propConfig->Item.ID, propConfig->Type);
        Delete(newProp);
        return NULL;
    }

    return newProp;
}


