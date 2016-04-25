/*
 * LogGlobalParam.h
 *
 *  Created on: 2014年8月1日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Util/TMTSingleton.h>
#include <string>

/*
 * LOG相关全局参数
 */
struct LogGlobalParam : public TMTSingleton<LogGlobalParam>
{
    LogGlobalParam(){}

    std::string         LogTag;
};


