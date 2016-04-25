/*
 * New2.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Object/NewCommon.h>

#define NEW2(Class, p0, p1)     NewCommon<Class>(); return new Class(p0, p1);

template <typename T, typename P0, typename P1>
T* New(P0& p0, P1& p1)              { NEW2(T, p0, p1) }

template <typename T, typename P0, typename P1>
T* New(const P0& p0, P1& p1)        { NEW2(T, p0, p1) }

template <typename T, typename P0, typename P1>
T* New(P0& p0, const P1& p1)        { NEW2(T, p0, p1) }

template <typename T, typename P0, typename P1>
T* New(const P0& p0, const P1& p1)  { NEW2(T, p0, p1) }

