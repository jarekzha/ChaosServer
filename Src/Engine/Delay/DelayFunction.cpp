/*
 * AsyncFunction.cpp
 *
 *  Created on: 2014年6月23日
 *      Author: jarek
 */

#include <Delay/DelayCenter.h>
#include <Delay/DelayFunction.h>
#include <Util/Util.h>


void DelayFunctionInvoker::Invoke(DelayFunction* func)
{
    ASSERT_RETURN_VOID(func != NULL);
    DelayCenter::Instance().PushDelayFunction(func);
}



