/*
 * AsyncEventCenter.h
 *
 *  Created on: 2014年6月21日
 *      Author: jarek
 *  异步事件中心:在update中，处理事件
 */

#pragma once

#include <Delay/DelayFunction.h>
#include <Thread/ThreadMutex.h>
#include <Util/TMTSingleton.h>
#include <queue>

class DelayFunction;

/*
 * delay中心。维护延迟调用函数
 */
class DelayCenter : public TMTSingleton<DelayCenter>
{
public:
                        DelayCenter();

    void                PushDelayFunction(DelayFunction* function);

    void                InvokeDelayFunction();

private:
    void                SwitchQueue(); //主从队列互换

private:
    typedef std::queue<DelayFunction*> DelayFunctionQueue;      // 延迟函数队列

    // 使用两个队列保存延迟函数，主备互换
    // 考虑到在延迟函数调用的过程中，发生PushDelayFunction
    DelayFunctionQueue  m_functionQueue[2];       // 延迟函数队列
    int                 m_masterQueueIndex;       // 主队列索引
    int                 m_slaveQueueIndex;        // 从队列索引

    ThreadMutex         m_mutext;
};



