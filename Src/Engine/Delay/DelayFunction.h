/*
 * AsyncFunction.h
 *
 *  Created on: 2014年6月23日
 *      Author: jarek
 */

#pragma once

/*
 * 延迟函数
 */
class DelayFunction
{
public:
    virtual             ~DelayFunction(){}

    virtual void        Invoke() = 0;
};


/*
 * 延迟函数调用者
 */
class DelayFunctionInvoker
{
public:
    static void         Invoke(DelayFunction* func);
};
