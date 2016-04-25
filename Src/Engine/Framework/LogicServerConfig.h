/*
 * LogicServerConfig.h
 *
 *  Created on: 2014年6月19日
 *      Author: jarek
 */

#pragma once

#include <Config/ConfigItem.h>
#include <Util/Util.h>
#include <string>

/*
 * logicserver的共有配置
 */
class LogicServerConfigItem : public ConfigItem
{
public:
    int                 ProcID;                 // 自己在ipc中的ID
    std::string         IPCConfigFile;          // ipc配置文件，相对于本配置文件的路径
    UINT                TimerWheelNum;          // 时间轮轮数
    UINT                TimerBucketNumPerWheel; // 时间轮单轮筒数
    UINT                TimerMSecondPerTick;    // 时间轮单个tick毫秒数

    // ConfigItem
public:
    virtual bool        LoadFromXmlElement(const TiXmlElement* element)
    {
        LOAD_XML_VALUE(element,     "ProcID",                   ProcID);
        LOAD_XML_STRING(element,    "IPCConfig",                IPCConfigFile);
        LOAD_XML_VALUE(element,     "TimerWheelNum",            TimerWheelNum);
        LOAD_XML_VALUE(element,     "TimerBucketNumPerWheel",   TimerBucketNumPerWheel);
        LOAD_XML_VALUE(element,     "TimerMSecondPerTick",      TimerMSecondPerTick);

        return true;
    }

    // LogicServerConfigItem
public:
    virtual void        DisplayConfig() const
    {
        LOG_RUN("ProcID:%d", ProcID);
        LOG_RUN("IPCConfig:%s", IPCConfigFile.c_str());
        LOG_RUN("TimerWheelNum:%u", TimerWheelNum);
        LOG_RUN("TimerBucketNumPerWheel:%u", TimerBucketNumPerWheel);
        LOG_RUN("TimerMSecondPerTick:%u", TimerMSecondPerTick);
    }
};
