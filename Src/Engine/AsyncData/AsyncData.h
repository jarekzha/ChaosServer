/*
 * AsyncData.h
 *
 *  Created on: 2014年7月4日
 *      Author: kcinz
 *      Modified by: Jarek     2015.11.5
 */

#pragma once
#include <AsyncData/AsyncDataDefine.h>
#include <Object/UniqueObject.h>
#include <Time/Timer.h>
#include <map>

class AsyncTask;

/**
 * 异步数据
 */
class AsyncData : public UniqueObject<AsyncData>
{
public:
    AsyncData();
    virtual ~AsyncData();

public:
    // 释放
    virtual void Release() = 0;

    // 请求异步加载
    virtual void OnStartLoad() = 0;

public:
    // 开始异步数据加载
    void StartLoad();

    // 结束加载
    void EndLoad();

    // 获取数据状态
    AsyncDataState GetDataState() const;

    int GetExpiresIntervalMS() const;
    void SetExpiresIntervalMS(int expiresInterval);

    int GetGetDataIntervalMS() const;
    void SetGetDataIntervalMS(int getDataInterval);

private:
    // 注册监听data的task
    void RegisterAsyncTask(AsyncTask& asyncTask);

    // 反注册data和task
    void UnregisterAsyncTask(AsyncTask& asyncTask);

    // 向监听task推送data
    void DispatchAsyncDataEvent(AsyncDataEvent asyncDataEvent);

    // Timer callback
private:
    // 当data获取超时
    void OnTimerGetDataTimeout();

    // 当data过期
    void OnTimerExpire();

private:
    friend class AsyncTask;
    typedef std::map<unsigned, AsyncTask*>    AsyncTaskMap;


private:
    AsyncDataState      state;              // 状态
    AsyncTaskMap        asyncTaskMap;       // 监听本data的task列表

    Timer               getDataTimeout;     // 获取超时
    int                 getDataIntervalMS;  // 获取超时间隔

    Timer               expireTimer;        // 过期计时器
    int                 expireIntervalMS;   // 过期间隔，由外部设定，若为0，则永不过期
};
