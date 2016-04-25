/*
 * Config.h
 *
 *  Created on: 2014年6月18日
 *      Author: jarek
 */

#pragma once

#include <Util/Util.h>
#include <Log/LogMacro.h>
#include <Config/TinyXmlUtil.h>
#include <boost/shared_ptr.hpp>

class TiXmlElement;

using boost::shared_ptr;

/*=============================================================================
 * 配置中的单条配置项
 */
class ConfigItem
{
public:
    virtual             ~ConfigItem(){}

    // 从一个xml项中读取字段
    virtual bool        LoadFromXmlElement(const TiXmlElement* element) = 0;

    // 获取ID，具体的configItem重写获取ID，有的没有ID，可以不管
    virtual UINT        GetID() const {return 0;}
};

/*=============================================================================
 * 读取配置的辅助宏，用于ConfigItem的子类在实现
 */
//--------------------------- 读取子元素----------------------------------------
// 读取值。读取数据强转成content的数据类型并传出。
#define LOAD_XML_VALUE(xmlElement, elementName, content) \
        if (!LoadValueElement(xmlElement, elementName, content)) \
        { \
            LOG_ERR("LoadXmlValue <Name:%s> <Value:%s> fail", elementName, xmlElement->Value()); \
            return false; \
        }

// 读取数字
#define LOAD_XML_NUMBER(xmlElement, elementName, content) \
        if (!LoadNumberElement(xmlElement, elementName, content)) \
        { \
            LOG_ERR("LoadXmlNumber <Name:%s> <Value:%s> fail", elementName, xmlElement->Value()); \
            return false; \
        }

// 读取string
#define LOAD_XML_STRING(xmlElement, elementName, content) \
        if (!LoadStringElement(xmlElement, elementName, content)) \
        { \
            LOG_ERR("LoadXmlString <Name:%s> <Value:%s> fail", elementName, xmlElement->Value()); \
            return false; \
        }

// 读取枚举。如果数据为空，则传出0。如果数据不为空，
// 通过传入的strToEnumFunc转换函数将其转换为对应枚举值并传出。
// strToEnumFunc -> EnumType (*typeStrToEnum)(const char*)
#define LOAD_XML_ENUM(xmlElement, elementName, content, strToEnumFunc) \
        if (!LoadEnumElement(xmlElement, elementName, content, strToEnumFunc)) \
        { \
            LOG_ERR("LoadXmlEnum <Name:%s> <Value:%s> fail", elementName, xmlElement->Value()); \
            return false; \
        }

//----------------------------读取属性---------------------------------------------
// 读取值
#define LOAD_XML_ATTR_VALUE(xmlElement, attributeName, content)     \
        if (!LoadValueArribute(xmlElement, attributeName, content)) \
        { \
            LOG_ERR("LoadXmlAttibuteValue <Element:%s> <Attr:%s> fail", xmlElement->Value(), attributeName); \
            return false; \
        }

// 读取数字
#define LOAD_XML_ATTR_NUMBER(xmlElement, attributeName, content)     \
        if (!LoadNumberArribute(xmlElement, attributeName, content)) \
        { \
            LOG_ERR("LoadXmlAttibuteNumber <Element:%s> <Attr:%s> fail", xmlElement->Value(), attributeName); \
            return false; \
        }

// 读取string
#define LOAD_XML_ATTR_STRING(xmlElement, attributeName, content) \
        if (!LoadStringAttribute(xmlElement, attributeName, content)) \
        { \
            LOG_ERR("LoadXmlAttibuteString <Element:%s> <Attr:%s> fail", xmlElement->Value(), attributeName); \
            return false; \
        }

// 读取枚举
// strToEnumFunc -> EnumType (*typeStrToEnum)(const char*)
#define LOAD_XML_ATTR_ENUM(xmlElement, attributeName, content, strToEnumFunc) \
        if (!LoadEnumAttribute(xmlElement, attributeName, content, strToEnumFunc)) \
        { \
            LOG_ERR("LoadXmlAttributeEnum <Element:%s> <Attr:%s> fail", xmlElement->Value(), attributeName); \
            return false; \
        }

