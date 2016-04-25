/*
 * ObjectStatistician.h
 *
 *  Created on: 2014年7月18日
 *  Author: jarek
 *  Brief: 对象统计
 */

#pragma once

#include <Util/TMTSingleton.h>
#include <Language/EnumConverter.h>
#include <map>
#include <string>

// 对象所处内存类型
struct ObjectMemType
{
    typedef enum
    {
        Heap = 0,
        Shm = 1,
        Max = 2
    } Enum;

    static void InitStringMap(EnumConvertMap<Enum>& map)
    {
        map.AddMap(Heap, "Heap");
        map.AddMap(Shm, "Shm");
    }

    typedef EnumConverter<Enum, InitStringMap> Conveter;
};

class ObjectInfo
{
public:
    ObjectInfo();

    int                 CurSingleCount(ObjectMemType::Enum memType);
    int                 CurArrayCount();

public:
    int                 NewSingleCount[ObjectMemType::Max];
    int                 DeleteSingleCount[ObjectMemType::Max];
    int                 NewArrayCount;
    int                 DeleteArrayCount;
};

class ObjectCounter : public TMTSingleton<ObjectCounter>
{
public:
                        ObjectCounter();
    virtual             ~ObjectCounter();

public:
    void                ChangeState(bool on);

    void                CountNewSingle(const char* className, ObjectMemType::Enum memType);

    void                CountDeleteSingle(const char* className, ObjectMemType::Enum memType);

    void                CountNewArray(const char* className);

    void                CountDeleteArray(const char* className);

    void                Print();

private:
    typedef std::map<std::string, ObjectInfo> ObjectInfoMap;
    ObjectInfoMap       objectInfoMap;

    bool                state;      // true->开启
};



