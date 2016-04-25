/*
 * xTinyXmlUtil.cpp
 *
 *  Created on: 2012-11-14
 *      Author: Peter_ZHA
 */

#include <Config/TinyXmlUtil.h>
#include <Util/Util.h>
#include <XML/tinyxml.h>
#include <string>
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;


bool LoadStringElement(const TiXmlElement* item, const char* elementName, std::string& content)
{
    ASSERT_RETURN(item != NULL && elementName != NULL, false);

    const TiXmlElement* tmpElement = item->FirstChildElement(elementName);
    if (NULL == tmpElement)
    {
        LOG_ERR("Element<name:%s> is not found", elementName);
        return false;
    }

    content = tmpElement->GetText() != NULL ? tmpElement->GetText() : "";
    return true;
}

bool LoadStringElement(const TiXmlElement* item, const char* elementName, std::vector<std::string>& content)
{
    return LoadElementToVector(item, elementName, content, LoadStringElement);
}

bool LoadStringAttribute(const TiXmlElement* item, const char* attributeName, std::string& content)
{
    ASSERT_RETURN(item != NULL && attributeName != NULL, false);

    const char* attrString = item->Attribute(attributeName);
    if (NULL == attrString)
    {
        LOG_ERR("Attribute<name:%s> is not found", attributeName);
        return false;
    }

    content = attrString;

    return true;
}

bool LoadStringAttribute(const TiXmlElement* item, const char* attributeName, std::vector<std::string>& content)
{
    return LoadAttributeToVector(item, attributeName, content, LoadStringAttribute);
}

TiXmlElement* FindFirstElement(TiXmlDocument& doc, const char* itemParent)
{
    ASSERT_RETURN(itemParent != NULL, NULL);

    const char* delim = ".";
    TiXmlElement* tempElement = NULL;
    vector<string> itemNameVec;
    boost::algorithm::split(itemNameVec, itemParent, boost::is_any_of(delim), boost::token_compress_on);

    for (vector<string>::iterator iter = itemNameVec.begin(); iter != itemNameVec.end(); ++iter)
    {
        if (iter == itemNameVec.begin())
        {
            tempElement = doc.FirstChildElement(iter->c_str());
        }
        else
        {
            tempElement = tempElement->FirstChildElement(iter->c_str());
        }

        ASSERT_RETURN(tempElement != NULL, NULL);
    }

    return tempElement;
}
