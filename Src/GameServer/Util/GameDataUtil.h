/*
 * GameDataUtil.h
 *
 *  Created on: 2014年8月11日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Config/ConfigFile.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Util/TMTSingleton.h>
#include <Framework/DataBinConfig.h>
#include <cassert>
#include <cstdio>
#include <string>

template <typename GameDataFile>
bool GameDataFileMultiLineInitialize(const std::string& dataName)
{
    std::string url = DataBinConfigFile::Instance().GetConfigUrl(dataName);
    if (url == DataBinConfigFile::Invalid)
    {
        LOG_ERR("GameData<%s> initialize fail, no url found", dataName.c_str());
        assert(false);
        return false;
    }

    url = DataBinConfigFile::Instance().GetFileDir() + url;

    ConfigFile& gameDataFile = GameDataFile::Instance();
    if (!gameDataFile.LoadFromXml(url.c_str(), "Root.Data"))
    {
        LOG_ERR("GameData<name%s, file:%s> load fail", dataName.c_str(), url.c_str());
        assert(false);
        return false;
    }

    LOG_RUN("Load game data<name:%s, path:%s> successfully", dataName.c_str(), url.c_str());

    return true;
}

template <typename GameDataFile>
bool GameDataFileSingleLineInitialize(const std::string& dataName)
{
    std::string url = DataBinConfigFile::Instance().GetConfigUrl(dataName);
    if (url == DataBinConfigFile::Invalid)
    {
        LOG_ERR("GameData<%s> initialize fail, no url found", dataName.c_str());
        assert(false);
        return false;
    }

    url = DataBinConfigFile::Instance().GetFileDir() + url;

    ConfigFile& gameDataFile = GameDataFile::Instance();
    if (!gameDataFile.LoadFromXml(url.c_str(), "Root.Data.Line"))
    {
        LOG_ERR("GameData<name%s, file:%s> load fail", dataName.c_str(), url.c_str());
        assert(false);
        return false;
    }

    LOG_RUN("Load game data<name:%s, path:%s> successfully", dataName.c_str(), url.c_str());

    return true;
}

