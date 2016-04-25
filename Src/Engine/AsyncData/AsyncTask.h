/*
 * AsyncTask.h
 *
 *  Created on: 2014年7月5日
 *      Author: kcinz
 *      Modified by: Jarek     2015.11.5
 */

#ifndef ASYNCTASK_H_
#define ASYNCTASK_H_

#include <AsyncData/AsyncData.h>
#include <list>
#include <map>

using std::list;
using std::map;

typedef list<AsyncData*> AsyncDataList;

/**
 * 异步任务，触发后自动释放
 */
class AsyncTask : public UniqueObject<AsyncTask>
{
public:
    AsyncTask();
    virtual ~AsyncTask();

protected:
    // 响应执行异步任务
    virtual void        OnExecute() = 0;

    // 异步数据获取超时
    virtual void        OnGetDataTimeOut() = 0;

    // 一定获得异步数据对象列表，由具体Task子类实现
    virtual void        OnGetCreateAsyncData(AsyncDataList& asyncDataList) = 0;

    // 释放
    virtual void        OnRelease() = 0;

public:
    // 开始执行，会先判断并开始加载异步数据
    void                StartExecuteAsyncTask();

private:
    // 处理异步数据事件
    void                HandleAsyncDataEvent(AsyncDataEvent event);

    // 注册AsyncTask需要的AsyncData
    void                RegisterAsyncData(AsyncData& asyncData);

    // 解耦AsyncTask和AsyncData的关系
    void                UnregisterAllAsyncData();

    // 异步数据加载完成
    void                OnAsyncDataComplete();

private:
    friend class AsyncData;
    typedef map<unsigned, AsyncData*> AsyncDataMap;

private:
    AsyncDataMap        asyncDataMap;
};

#endif /* ASYNCTASK_H_ */
