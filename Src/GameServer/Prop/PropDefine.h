/*
 * PropDefine.h
 *
 *  Created on: 2014年8月26日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Language/EnumConverter.h>

/*
 * 道具类型
 */
struct PropType
{
    typedef enum
    {
        Common = 1,     // 普通道具：灵魂石、卷轴、军师技能的碎片、各种消耗品
        AddExp = 2,     // 添加经验道具
    } Enum;

    static void InitStringMap(EnumConvertMap<Enum>& map)
    {
        map.AddMap(Common, "Common");
        map.AddMap(AddExp, "AddExp");
    }

    typedef EnumConverter<Enum, InitStringMap> Converter;
};

