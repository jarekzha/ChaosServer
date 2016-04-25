/*
 * ProtobufError.h
 *
 *  Created on: 2014年6月24日
 *      Author: jarek
 */

#pragma once

#include <google/protobuf/repeated_field.h>
#include <google/protobuf/stubs/common.h>
#include <Log/LogMacro.h>
#include <string>

inline void MyLogHandler(google::protobuf::LogLevel level, const char* filename, int line,
                        const std::string& message)
{
    LOG_ERR("Protobuf error <level:%d> <detail:%s> <file:%s> <line:%d>",
            (int)level, message.c_str(), filename, line);
}

inline void InitProtobufLogErr()
{
    LOG_RUN("Set protobuf log err handler");
    google::protobuf::SetLogHandler(MyLogHandler);
}

using ::google::protobuf::RepeatedPtrField;
