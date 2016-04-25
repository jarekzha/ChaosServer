/*
 * DBUtil.cpp
 *
 *  Created on: 2014年8月9日
 *  Author: jarek
 *  Brief: 
 */

#include <stddef.h>
#include <Util/DBUtil.h>
#include <string>

using namespace std;


BinaryBuff::BinaryBuff(const string& binaryStr)
{
    char* binaryStart = (char*)binaryStr.c_str();
    setg(binaryStart, binaryStart, binaryStart + binaryStr.size());
}

BinaryBuff::BinaryBuff(char* buff, size_t size)
{
    setg(buff, buff, buff + size);
}

string ReadString(istream& inStream)
{
    string outString;
    while (inStream.good())
    {
        char c = inStream.get();
        if (inStream.good())
        {
            outString += c;
        }
    }

    return outString;
}
