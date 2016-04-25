/*
 * EnumConvertible.h
 *
 *  Created on: 2014年7月12日
 *  Author: jarek
 *  Brief: 枚举类型转换器
 */

#pragma once

#include <map>
#include <string>
#include <assert.h>

/*
 * 枚举转换映射表
 */
template<typename Enum>
class EnumConvertMap
{
public:
    typedef Enum EnumType;
    static const std::string InvalidString;
    static const int InvalidEnum;

public:
    const std::string& EnumToString(EnumType e)
    {
        typename EnumToStringMap::iterator iter = enumToStringMap.find(e);
        if (iter == enumToStringMap.end())
        {
            return InvalidString;
        }
        else
        {
            return iter->second;
        }
    }

    EnumType StringToEnum(const std::string& str)
    {
        typename StringToEnumMap::iterator iter = stringToEnumMap.find(str);
        if (iter == stringToEnumMap.end())
        {
            return InvalidEnum;
        }
        else
        {
            return iter->second;
        }
    }

    void AddMap(EnumType e, const std::string& str)
    {
        enumToStringMap[e] = str;
        stringToEnumMap[str] = e;
    }

private:
    typedef std::map<EnumType, std::string> EnumToStringMap;
    typedef std::map<std::string, EnumType> StringToEnumMap;

    EnumToStringMap enumToStringMap;
    StringToEnumMap stringToEnumMap;
};

template<typename Enum>
const std::string EnumConvertMap<Enum>::InvalidString = "ValidEnum";

template<typename Enum>
const int EnumConvertMap<Enum>::InvalidEnum = -1;

/*
 * 枚举转换器，static类
 */
template<typename Enum, void (*initMap)(EnumConvertMap<Enum>& map)>
class EnumConverter
{
public:
    typedef Enum EnumType;

public:
    static const std::string& EnumToString(EnumType e)
    {
        Initialize();
        return enumMap.EnumToString(e);
    }

    static EnumType StringToEnum(std::string& str)
    {
        Initialize();
        return enumMap.StringToEnum(str);
    }

    static bool Valid(EnumType e)
    {
        return EnumToString(e) != enumMap.InvalidString;
    }

    static bool Valid(std::string str)
    {
        return StringToEnum(str) != enumMap.InvalidEnum;
    }

private:
    static void Initialize()
    {
        if (initialize)
        {
            return;
        }

        initMap(enumMap);
        initialize = true;
    }

private:
    static EnumConvertMap<Enum> enumMap;
    static bool initialize;
};

template<typename Enum, void (*initMap)(EnumConvertMap<Enum>& map)>
EnumConvertMap<Enum> EnumConverter<Enum, initMap>::enumMap;

template<typename Enum, void (*initMap)(EnumConvertMap<Enum>& map)>
bool EnumConverter<Enum, initMap>::initialize;

//////////////////////////// 使用例子 /////////////////////////////////////////////
/* 例子1
 typedef enum
 {
 Male = 1,
 Female = 2,
 } Player;

 void PlayerEnumPairDefine(EnumConvertMap<Player>& enumMap)
 {
 enumMap.AddMap(Male, "Male");
 }

 typedef EnumConverter<Player, EnumPairDefine> PlayerConverter;
 */

/* 例子2
 struct Color
 {
 typedef enum
 {
 Blue,
 Green,
 } Enum;

 static void EnumPairDefine(EnumConvertMap<Enum>& enumMap)
 {
 enumMap.AddMap(Blue, "Blue");
 enumMap.AddMap(Green, "Green");
 }

 typedef EnumConverter<Enum, EnumPairDefine> Converter;
 };
 */
