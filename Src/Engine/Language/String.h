#pragma once

#include <string.h>
#include <string>
#include <Util/Util.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <iomanip>
#include <stdlib.h>


class String
{
public:
    static UINT         BKDRHash(char *str)
    {
        UINT seed = 131; // 31 131 1313 13131 131313 etc..
        UINT hash = 0;
        while (*str)
        {
            hash = hash * seed + (*str++);
        }
        return (hash & 0x7FFFFFFF);
    }
    
};

inline char* StrCpy(char* dest, const char* src, size_t size)
{
    if (size == 0)
    {
        return dest;
    }
    
    char* result = strncpy(dest, src, size);
    dest[size - 1] = 0;
    
    return result;
}

template <typename Target, typename Source>
inline Target LexicalCast(const Source& source)
{
    return boost::lexical_cast<Target>(source);
}

template <typename Source>
inline std::string ToString(const Source& source)
{
    std::string ret;
    try
    {
        ret = boost::lexical_cast<std::string>(source);
    }
    catch(...)
    {
        return "";
    }

    return ret;
}


// 二进制转换成string
inline std::string BinaryToReadableString(const char* buff, size_t len)
{
    std::stringstream oStream;

    for(size_t i = 0; i < len; i++)
    {
        unsigned char byte = buff[i];
        oStream << std::hex << std::setfill('0') << std::setw(2) << (int)byte << ' ';
    }

    return oStream.str();
}

// 检测字符串对应数为整数
inline bool CheckInteger(const char* numberString)
{
    const char* c = numberString;
    if (!((*c >= '0' && *c <= '9') || *c == '-'))
    {
        return false;
    }

    ++c;

    while (*c != 0)
    {
        if (*c < '0' || *c >'9')
        {
            return false;
        }

        ++c;
    }

    return true;
}



