/*
 * xTinyXmlUtil.h
 * 用tinyxml读取xml字段时的一些辅助函数
 *  Created on: 2012-11-13
 *      Author: Peter_ZHA
 */

#pragma once
#include <XML/tinyxml.h>
#include <Util/Util.h>
#include <vector>
#include <string>
#include <boost/lexical_cast.hpp>
#include <Log/LogMacro.h>
#include <Language/String.h>

/**
 * 调用读取函数，读取元素到vector中
 * 读取规则，elementName + 序号，序号从1开始
 */
template <typename ValueType>
bool LoadElementToVector(const TiXmlElement* item,
                         const char* elementName,
                         std::vector<ValueType>& content,
                         bool (LoadFunc)(const TiXmlElement* item, const char* elementName, ValueType& content))
{
    ASSERT_RETURN(item != NULL && elementName != NULL, false);

    int i = 0;
    std::string elementNameBase = elementName;
    while (true)
    {
        ++i;
        std::string tmpElementName = elementNameBase + boost::lexical_cast<std::string>(i);
        const TiXmlElement* tmpElement = item->FirstChildElement(tmpElementName.c_str());
        if (NULL == tmpElement)
        {
            break;
        }

        ValueType value;
        if (!LoadFunc(item, tmpElementName.c_str(), value))
        {
            return false;
        }

        content.push_back(value);
    }

    return true;
}

/**
 * 调用读取函数，读取属性到vector中
 * 读取规则，elementName + 序号，序号从1开始
 */
template <typename ValueType>
bool LoadAttributeToVector(const TiXmlElement* item,
                         const char* attributeName,
                         std::vector<ValueType>& content,
                         bool (LoadFunc)(const TiXmlElement* item, const char* attributeName, ValueType& content))
{
    ASSERT_RETURN(item != NULL && attributeName != NULL, false);

    int i = 0;
    std::string attrNameBase = attributeName;
    while (true)
    {
        ++i;
        std::string tmpAttrName = attrNameBase + boost::lexical_cast<std::string>(i);
        const char* attrString = item->Attribute(tmpAttrName.c_str());
        if (NULL == attrString)
        {
            break;
        }

        ValueType value;
        if (!LoadFunc(item, tmpAttrName.c_str(), value))
        {
            return false;
        }

        content.push_back(value);
    }

    return true;
}

//================================= 读取子element ================================
/**
 * 读取子element值
 */
template <typename ValueType>
bool LoadValueElement(const TiXmlElement* item, const char* elementName, ValueType& content)
{
    ASSERT_RETURN(item != NULL && elementName != NULL, false);

    const TiXmlElement* tmpElement = item->FirstChildElement(elementName);
    if (NULL == tmpElement)
    {
        LOG_ERR("Element<name:%s> is not found", elementName);
        return false;
    }

    content = ValueType(tmpElement->GetText() != NULL ?
            atoi(tmpElement->GetText()) : 0);
    return true;
}

/**
 * 读取子element值到vector中
 */
template <typename ValueType>
bool LoadValueElement(const TiXmlElement* item, const char* elementName, std::vector<ValueType>& content)
{
    return LoadElementToVector(item, elementName, content, LoadValueElement<ValueType>);
}

/**
 * 读取子element的数字
 */
template <typename ValueType>
bool LoadNumberElement(const TiXmlElement* item, const char* elementName, ValueType& content)
{
    ASSERT_RETURN(item != NULL && elementName != NULL, false);

    const TiXmlElement* tmpElement = item->FirstChildElement(elementName);
    if (NULL == tmpElement)
    {
        LOG_ERR("Element<name:%s> is not found", elementName);
        return false;
    }

    try
    {
        content = LexicalCast<ValueType>(tmpElement->GetText());
    }
    catch(...)
    {
        return false;
    }
    return true;
}

/**
 * 读取子element数字值到vector中
 */
template <typename ValueType>
bool LoadNumberElement(const TiXmlElement* item, const char* elementName, std::vector<ValueType>& content)
{
    return LoadElementToVector(item, elementName, content, LoadNumberElement<ValueType>);
}

/**
 * 读取子element字符串
 */
extern bool LoadStringElement(const TiXmlElement* item, const char* elementName, std::string& content);

/**
 * 读取子element字符串到vector中
 */
extern bool LoadStringElement(const TiXmlElement* item, const char* elementName, std::vector<std::string>& content);

/**
 * 读取子element的值并转换为预定义的枚举
 */
template <typename EnumType>
bool LoadEnumElement(const TiXmlElement* item, const char* elementName,
        EnumType& content, EnumType (*typeStrToEnum)(const char*))
{
    ASSERT_RETURN(item != NULL && elementName != NULL && typeStrToEnum != NULL, false);

    std::string enumStr;
    if (!LoadStringElement(item, elementName, enumStr))
    {
        return false;
    }

    if (enumStr == "")
    {
        content = EnumType(0);
        return true;
    }

    content = typeStrToEnum(enumStr.c_str());
    return true;
}

/**
 * 读取子element的值并转换为预定义的枚举到vector中
 */
template <typename EnumType>
bool LoadEnumElement(const TiXmlElement* item, const char* elementName,
        std::vector<EnumType>& content, EnumType (*typeStrToEnum)(const char*))
{

    ASSERT_RETURN(item != NULL && elementName != NULL && typeStrToEnum != NULL, false);

    int i = 0;
    std::string elementNameBase = elementName;
    while (true)
    {
        ++i;
        std::string tmpElementName = elementNameBase + boost::lexical_cast<std::string>(i);
        const TiXmlElement* tmpElement = item->FirstChildElement(tmpElementName.c_str());
        if (NULL == tmpElement)
        {
            break;
        }

        EnumType value;
        if (!LoadEnumElement(item, tmpElementName.c_str(), value, typeStrToEnum))
        {
            return false;
        }

        content.push_back(value);
    }

    return true;
}

//===================================== 读取element属性============================
/**
 * 读取element的属性值
 */
template <typename ValueType>
bool LoadValueArribute(const TiXmlElement* item, const char* attributeName, ValueType& content)
{
    ASSERT_RETURN(item != NULL && attributeName != NULL, false);

    const char* attrString = item->Attribute(attributeName);
    if (NULL == attrString)
    {
        LOG_ERR("Attribute<name:%s> is not found", attributeName);
        return false;
    }

    content = (ValueType)atoi(attrString);
    return true;
}

/**
 * 读取element的属性值到vector中
 */
template <typename ValueType>
bool LoadValueArribute(const TiXmlElement* item, const char* attributeName, std::vector<ValueType>& content)
{
    return LoadAttributeToVector(item, attributeName, content, LoadValueArribute<ValueType>);
}

/**
 * 读取element的属性数字
 */
template <typename ValueType>
bool LoadNumberArribute(const TiXmlElement* item, const char* attributeName, ValueType& content)
{
    ASSERT_RETURN(item != NULL && attributeName != NULL, false);

    const char* attrString = item->Attribute(attributeName);
    if (NULL == attrString)
    {
        LOG_ERR("Attribute<name:%s> is not found", attributeName);
        return false;
    }

    try
    {
        content = LexicalCast<ValueType>(attrString);
    }
    catch (...)
    {
        return false;
    }

    return true;
}

/**
 * 读取element的属性数字到vector中
 */
template <typename ValueType>
bool LoadNumberArribute(const TiXmlElement* item, const char* attributeName, std::vector<ValueType>& content)
{
    return LoadAttributeToVector(item, attributeName, content, LoadNumberArribute<ValueType>);
}

/*
 * 读取element的属性字符串
 */
extern bool LoadStringAttribute(const TiXmlElement* item, const char* attributeName, std::string& content);

/*
 * 读取element的属性字符串到vector中
 */
extern bool LoadStringAttribute(const TiXmlElement* item, const char* attributeName, std::vector<std::string>& content);


/**
 * 读取子element的属性，转换为中预定义的枚举
 */
template <typename EnumType>
bool LoadEnumAttribute(const TiXmlElement* item, const char* attributeName,
        EnumType& content, EnumType (*typeStrToEnum)(const char*))
{
    ASSERT_RETURN(item != NULL && attributeName != NULL && typeStrToEnum != NULL, false);

    std::string enumStr;
    if (!LoadStringAttribute(item, attributeName, enumStr))
    {
        return false;
    }

    if (enumStr == "")
    {
        content = EnumType(0);
        return true;
    }

    content = typeStrToEnum(enumStr.c_str());
    return true;
}

/**
 * 读取子element的属性，转换为中预定义的枚举到vector中
 */
template <typename EnumType>
bool LoadEnumAttribute(const TiXmlElement* item, const char* attributeName,
        std::vector<EnumType>& content, EnumType (*typeStrToEnum)(const char*))
{
    ASSERT_RETURN(item != NULL && attributeName != NULL && typeStrToEnum != NULL, false);

    int i = 0;
    std::string attrNameBase = attributeName;
    while (true)
    {
        ++i;
        std::string tmpAttrName = attrNameBase + boost::lexical_cast<std::string>(i);
        const char* attrString = item->Attribute(attributeName);
        if (NULL == attrString)
        {
            break;
        }

        EnumType value;
        if (!LoadEnumAttribute(item, tmpAttrName.c_str(), value, typeStrToEnum))
        {
            return false;
        }

        content.push_back(value);
    }

    return true;
}

/**
 * 查找xml文件的首个item
 */
extern TiXmlElement* FindFirstElement(TiXmlDocument& doc, const char* itemParent);

