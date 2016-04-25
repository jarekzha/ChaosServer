/*
 * LogicServerHelper.h
 *
 *  Created on: 2014年6月19日
 *      Author: jarek
 */

#pragma once
#include <string>

class LogicServerConfigItem;
class ConfigFile;

/**
 * 逻辑服务器助手，提供一些接口供logicServer调用，也被上层逻辑调用
 */
class LogicServerExtendable
{
protected:
    virtual             ~LogicServerExtendable(){}

    // 载入配置文件
    virtual bool        LoadConfigFile(const std::string& configFilePath,
                                       const LogicServerConfigItem*& outLogicServerConfigItem,
                                       const ConfigFile*& outConfigFile) = 0;

    // 初始化消息层
    virtual bool        InitMessageLayer() = 0;

    // 初始化线程管理器
    virtual bool        InitThreadManager() = 0;

    // 初始化扩展组件
    virtual bool        InitExtraComponent() = 0;

protected:
    std::string         configFile;
};

