/*
 * New3.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#define NEW3(Class, p0, p1, p2)     NewCommon<Class>(); return new Class(p0, p1, p2);

template <typename T, typename P0, typename P1, typename P2>
T* New(P0& p0, P1& p1, P2& p2)                    { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(const P0& p0, P1& p1, P2& p2)              { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(P0& p0, const P1& p1, P2& p2)              { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(const P0& p0, const P1& p1, P2& p2)        { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(P0& p0, P1& p1, const P2& p2)              { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(const P0& p0, P1& p1, const P2& p2)        { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(P0& p0, const P1& p1, const P2& p2)        { NEW3(T, p0, p1, p2) }

template <typename T, typename P0, typename P1, typename P2>
T* New(const P0& p0, const P1& p1, const P2& p2)  { NEW3(T, p0, p1, p2) }

