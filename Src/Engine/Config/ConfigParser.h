/*
 * ConfigParser.h
 *
 *  Created on: 2014年8月14日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

/*
 * 配置分析器，用于分析ConfigFileMultiItem
 */
class ConfigParser
{
public:
    virtual             ~ConfigParser(){}

    virtual bool        Parse() = 0;

    virtual void        Reset() = 0;
};

