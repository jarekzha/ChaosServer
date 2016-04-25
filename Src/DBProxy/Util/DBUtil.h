/*
 * DBUtil.h
 *
 *  Created on: 2014年8月8日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <stddef.h>
#include <iostream>
#include <string>


class BinaryBuff : public std::streambuf
{
public:
    BinaryBuff(const std::string& binaryStr);
    BinaryBuff(char* buff, size_t size);
};


extern std::string ReadString(std::istream& inStream);

