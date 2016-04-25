/*
 * New4.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Object/NewCommon.h>

#define NEW4(Class, p0, p1, p2, p3)     NewCommon<Class>(); return new Class(p0, p1, p2, p3);

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, P1& p1, P2& p2, P3& p3)                    { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, P1& p1, P2& p2, P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, const P1& p1, P2& p2, P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, const P1& p1, P2& p2, P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, P1& p1, const P2& p2, P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, P1& p1, const P2& p2, P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, const P1& p1, const P2& p2, P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, const P1& p1, const P2& p2, P3& p3)  { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, P1& p1, P2& p2, const P3& p3)                    { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, P1& p1, P2& p2, const P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, const P1& p1, P2& p2, const P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, const P1& p1, P2& p2, const P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, P1& p1, const P2& p2, const P3& p3)              { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, P1& p1, const P2& p2, const P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(P0& p0, const P1& p1, const P2& p2, const P3& p3)        { NEW4(T, p0, p1, p2, p3) }

template <typename T, typename P0, typename P1, typename P2, typename P3>
T* New(const P0& p0, const P1& p1, const P2& p2, const P3& p3)  { NEW4(T, p0, p1, p2, p3) }


