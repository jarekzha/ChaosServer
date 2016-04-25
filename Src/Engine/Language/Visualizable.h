/*
 * Visualizable.h
 *
 *  Created on: 2014年7月21日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <string>
#include <sstream>

class Visualizable
{
public:
    Visualizable(){}
    virtual             ~Visualizable(){}

public:
    // 简要信息
    virtual std::string BriefInfo() const = 0;

    // 详细信息
    virtual std::string DetailInfo() const
    {
        return BriefInfo();
    }
};




