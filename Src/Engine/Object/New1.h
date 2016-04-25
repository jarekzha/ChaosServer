/*
 * New1.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Object/NewCommon.h>


#define NEW1(Class, p0)     NewCommon<Class>(); return new Class(p0);

template <typename T, typename P0>
T* New(P0& p0)              { NEW1(T, p0) }

template <typename T, typename P0>
T* New(const P0& p0)        { NEW1(T, p0) }

