/*
 * xTypeID.h
 * type ID生成器
 *
 * 用法：
 *  xTypeID<ClassType>::ID()的值因Type不同而不同，
 *  从而可用于自动生成class的唯一ID
 *
 *  @@@@注意：同一Type生成的ID会因代码版本的不同而不同。@@@@
 *
 *  Created on: 2012-11-11
 *      Author: Peter_ZHA
 */

#pragma once
#include "Util/Util.h"
#include <string>
#include <cxxabi.h>

extern UINT GetNewID();

/**
 * 获取静态类型的ID和名称
 */
template <typename Type>
class TypeID
{
public:
    static const UINT           ID;
    static const std::string    Name;

private:
    static std::string TypeName()
    {
        char buffer[1024];
        size_t size = sizeof(buffer);
        int status = 0;
        const char* name = typeid(Type).name();
        char* realName = abi::__cxa_demangle(name, buffer, &size, &status);
        return std::string(realName);
    }
};

template <typename Type>
const UINT TypeID<Type>::ID = GetNewID();

template <typename Type>
const std::string TypeID<Type>::Name = TypeID<Type>::TypeName();


/**
 * 获取变量的真实类型的名称
 */
template <typename Type>
std::string VariableTypeName(const Type& value)
{
    char buffer[1024];
    size_t size = sizeof(buffer);
    int status = 0;
    const char* name = typeid(value).name();
    char* realName = abi::__cxa_demangle(name, buffer, &size, &status);
    return std::string(realName);
}




