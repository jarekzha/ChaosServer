/*
 * GMMessage.cpp
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#include "GMMessage.h"
#include <Message/MessageDefine.h>
#include <Engine/MessageLayer/OriginalData.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>

GMMessage::GMMessage()
    :httpRequestContent("")
{
    // NULL
}

GMMessage::~GMMessage()
{
    // NULL
}

bool GMMessage::InitMsg(OriginalData * data)
{
    httpRequestContent.assign(data->GetBuff(), data->GetBuffLen());

    return true;
}

const std::string& GMMessage::GetHttpRequestContent() const
{
    return httpRequestContent;
}
