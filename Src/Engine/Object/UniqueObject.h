/*
 * Object.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Util/Util.h>

/*
 * 在游戏中大量存在的，需要ID进行唯一性索引的对象
 */
template <typename Class>
class UniqueObject
{
public:
    typedef unsigned   IDType;

public:
    UniqueObject():objectID(GetNewID()){}
    virtual             ~UniqueObject();

    // UniqueObject
public:
    virtual IDType      GetObjectID() const
    {
        return objectID;
    }

private:
    static IDType       GetNewID()
    {
        static IDType id = 0;
        return ++id;
    }

private:
    IDType              objectID;
};









