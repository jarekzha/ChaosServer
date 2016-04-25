/*
 * DataBinConfig.cpp
 *
 *  Created on: 2014年8月11日
 *  Author: jarek
 *  Brief:
 */

#include <Framework/DataBinConfig.h>
#include <string>

using namespace std;

const string DataBinConfigFile::Invalid="";

bool DataBinConfigFile::LoadFromXml(const char* file)
{
    if (!ConfigFile::LoadFromXml(file))
    {
        return false;
    }

    TiXmlDocument configXml;
    if (!configXml.LoadFile(file))
    {
        LOG_ERR("Load xml <file:%s> fail: %s", file, configXml.ErrorDesc());
        return false;
    }

    TiXmlElement* rootElement = configXml.RootElement();
    ASSERT_RETURN(rootElement != NULL, false);

    TiXmlElement* element = rootElement->FirstChildElement();
    while (element != NULL)
    {
        DataBinConfigItem* configItem = new DataBinConfigItem;
        ASSERT_RETURN(configItem != NULL, false);

        if (!configItem->LoadFromXmlElement(element))
        {
            assert(false);
            element = element->NextSiblingElement();
            delete configItem;
            LOG_ERR("Load <conf:%s> <item:%s> fail", file, element->GetText());
            continue;
        }

        nameDataBinMap[configItem->Name] = configItem;
        element = element->NextSiblingElement();
    }

    return true;
}


const std::string& DataBinConfigFile::GetConfigUrl(
        const std::string& dataBinName)
{
    NameDataBinMap::iterator iter = nameDataBinMap.find(dataBinName);
    if (iter == nameDataBinMap.end())
    {
        return Invalid;
    }

    return iter->second->Url;
}




