/*
 * NewCommon.h
 *
 *  Created on: 2014年8月27日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Util/TMTSingleton.h>
#include <Language/TypeID.h>
#include <Object/ObjectCounter.h>

/**
 * new的公共操作
 */
template <typename T>
void NewCommon()
{
    ObjectCounter::Instance().CountNewSingle(TypeID<T>::Name.c_str(), ObjectMemType::Heap);
}
