/*
 * HTTPUtil.cpp
 *
 *  Created on: 2014年8月31日
 *      Author: silas
 */

#include <Util/HTTPUtil.h>
#define MAX_DECODE_BUFLEN (1024)

HTTPUtil::HTTPUtil()
{
    // NULL
}

HTTPUtil::~HTTPUtil()
{
    // NULL
}

bool HTTPUtil::DecodeHTTPUrlString(const char* encodeUrl, int encodeUrlLen, std::string& decodeUrl)
{
    if (NULL == encodeUrl || encodeUrlLen > MAX_DECODE_BUFLEN)
    {
        return false;
    }

    char decodeBuf[MAX_DECODE_BUFLEN] = {0};
    for(int i = 0, j = 0; i <= encodeUrlLen; i++, j++)
    {
      if(encodeUrl[i] == '+')
      {
          decodeBuf[j] = ' ';
      }
      else if(encodeUrl[i] == '%')
      {
          decodeBuf[j] = TranslateHex(encodeUrl[i + 1]) * 16 + TranslateHex(encodeUrl[i + 2]);
          i += 2; // Move past hex code
      }
      else    // An ordinary character
      {
          decodeBuf[j] = encodeUrl[i];
      }
    }

    decodeUrl = decodeBuf;
    return true;
}

bool HTTPUtil::Split(const std::string& srcStr, const std::string& separator, std::vector<std::string>& strVec)
{
    if (srcStr.empty() || separator.empty())
    {
        return false;
    }

    std::string orignalStr(srcStr);
    std::string subStr("");
    std::string::size_type start = 0, index = 0;

    do
    {
        index = orignalStr.find_first_of(separator, start);
        if (std::string::npos != index)
        {
            subStr = orignalStr.substr(start, index - start);
            strVec.push_back(subStr);

            start = orignalStr.find_first_not_of(separator,index);

            if (start == std::string::npos)
            {
                return true;
            }
        }
    } while (index != std::string::npos);

    //the last token
    subStr = orignalStr.substr(start);
    strVec.push_back(subStr);

    return true;
}

char HTTPUtil::TranslateHex(char hex)
{
    if(hex >= 'A')
    {
        return (hex & 0xdf) - 'A' + 10;
    }
    else
    {
        return hex - '0';
    }
}
