/*
 * New0.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Object/NewCommon.h>

template <typename T>
T* New()
{
    NewCommon<T>();
    return new T;
}

