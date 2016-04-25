/*
 * Prop.cpp
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Object/New.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>
#include <Prop/Config/PropConfig.h>
#include <Prop/Prop/Prop.h>
#include <Protocol/Macro.pb.h>
#include <cstdio>


Prop::Prop()
    : BagItem(BAG_ITEM_TYPE_PROP)
{
    resID = 0;
}

Prop::~Prop()
{
    // NULL
}

UINT Prop::GetBagItemResID() const
{
    return resID;
}

bool Prop::Initialize(UINT resID)
{
    this->resID = resID;
    config = PropConfigFile::Instance().Indexer.GetItemToHold(resID);
    if (config.get() == NULL)
    {
        LOG_ERR("Prop<%s> Initialize fail", BriefInfo());
        return false;
    }

    return true;
}

PropType::Enum Prop::GetPropType() const
{
    ASSERT_RETURN(config.get() != NULL, PropType::Common);
    return config->Type;
}

BagItem* Prop::Clone() const
{
    return New<Prop>(*this);
}

