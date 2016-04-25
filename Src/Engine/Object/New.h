/*
 * New.h
 *
 *  Created on: 2014年7月12日
 *  Author: jarek
 *  Brief: 自定义new和delete，替换对原生new和delete的直接使用。中间可以插入部分自定义操作
 */

#pragma once

#include <Object/NewCommon.h>

//================================= New 单一对象=======================================
// 多个模版实现，满足不用的构造函数需求
// 使用例子 Class obj = New<Class>(param1, param2)
#include <Object/New0.h>
#include <Object/New1.h>
#include <Object/New2.h>
#include <Object/New3.h>
#include <Object/New4.h>
#include <Object/New5.h>

/*
 * 删除单一对象
 * 使用例子:Delete(object)
 */
template <typename T>
void Delete(T*& ptr)
{
    if (NULL == ptr)
    {
        return;
    }

    ObjectCounter::Instance().CountDeleteSingle(VariableTypeName(*ptr).c_str(), ObjectMemType::Heap);
    delete ptr;
    ptr = 0;
}

/*
 * New对象数组
 * 使用例子: Class* array = NewArray<Class>(10);
 */
template <typename T>
T* NewArray(int count)
{
    ObjectCounter::Instance().CountNewArray(TypeID<T>::Name.c_str());
    return new T[count];
}

/*
 * 删除对象数组
 * 使用例子：DeleteArray(array);
 */
template <typename T>
void DeleteArray(T*& ptr)
{
    if (NULL == ptr)
    {
        return;
    }

    ObjectCounter::Instance().CountDeleteArray(VariableTypeName(*ptr).c_str());
    delete[] ptr;
    ptr = NULL;
}
