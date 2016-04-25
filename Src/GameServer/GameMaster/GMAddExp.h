/*
 * GMAddExp.h
 *
 *  Created on: 2014年10月20日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <GameMaster/GMCommand.h>

class GMAddExp : public GMCommand
{
public:
    GMAddExp() {}
    virtual             ~GMAddExp() {}

    // GMCommand
public:
    virtual bool        HandleCommand(std::vector<std::string>& httpCommandParamVec);
};



