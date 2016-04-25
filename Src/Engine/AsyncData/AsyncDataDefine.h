/*
 * AsyncDataDefine.h
 *
 *  Created on: 2015年11月5日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

/*
 * 异步数据状态
 */
enum AsyncDataState
{
    AsyncDataState_Empty    = 0,        // 空状态
    AsyncDataState_Getting  = 1,        // 获取状态
    AsyncDataState_Complete = 2,        // 完成状态
};

/*
 * 异步数据事件
 */
enum AsyncDataEvent
{
    AsyncDataEvent_GetTimeout   = 0,    // 异步获取数据超时
    AsyncDataEvent_GetComplete  = 1,    // 异步获取数据完成
};

static const unsigned int GET_ASYNC_DATA_TIMEOUT_MS = (5*60*1000);         // 获取异步数据5min超时
static const unsigned int CACHE_ASYNC_DATA_TIMEOUT_MS = (60*60*1000);   // 缓存1小时时间，超时就释放


