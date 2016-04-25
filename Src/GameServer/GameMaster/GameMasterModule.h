/*
 * GameMasterModule.h
 *
 *  Created on: 2014年9月1日
 *      Author: silas
 */

#pragma once
#include <Engine/Framework/Module.h>
#include <vector>
#include <map>
#include <string>

class GMCommand;

/**
 * GM模块
 */
class GameMasterModule : public LogicModule
{
public:
    GameMasterModule();
    virtual ~GameMasterModule();

    // Module
public:
    virtual bool        Initialize();

    // GameMasterModule
public:
    /*
     * @brief : 处理GM请求
     *
     * @param httpCommandParamVec : GM参数vector
     *
     * @return : 处理结果[true : 成功 false : 失败]
     *
     * */
    bool                HandleGMRequest(std::vector<std::string>& httpCommandParamVec);

private:
    typedef std::map<std::string, GMCommand*> GMCommandHanlderMap;

private:
    GMCommandHanlderMap gmCommandHandlerMap;
};

