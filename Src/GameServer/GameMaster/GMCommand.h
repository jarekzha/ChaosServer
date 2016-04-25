/*
 * GMCommand.h
 *
 *  Created on: 2014年9月4日
 *      Author: silas
 */

#pragma once

#include <vector>
#include <map>
#include <string>

class GMCommand
{
public:
    virtual             ~GMCommand(){}

    virtual bool        HandleCommand(std::vector<std::string>& httpCommandParamVec) = 0;
};
