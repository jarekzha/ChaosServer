/*
 * GMAddItem.h
 *
 *  Created on: 2014年10月20日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <GameMaster/GMCommand.h>

/**
 * add item[equipment/prop/fragment/hero...]
 */
class GMAddItem : public GMCommand
{
public:
    GMAddItem();
    virtual             ~GMAddItem();

    //GMCommand
public:
    virtual bool        HandleCommand(std::vector<std::string>& httpCommandParamVec);

private:
    typedef std::map<std::string, int/*item-type*/> ITEMTYPEMAP;

private:
    ITEMTYPEMAP         itemTypeMap;
};


