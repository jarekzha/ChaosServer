#pragma once
#include <stdio.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <Util/Assert.h>

#define SAFE_DELETE(p)  if (p != NULL ) { delete p; p = NULL;}
#define SAFE_DELETE_ARRAY(p)  if (p != NULL ) { delete[] p; p = NULL;}

typedef unsigned int        UINT;
typedef unsigned long       ULONG;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;


#define SECOND_OF_DAY      (24 * 3600)      // 单天描述

using boost::shared_ptr;

// 开启消息trace日志
#define MSG_TRACE_ON        0

