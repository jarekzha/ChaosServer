/*
 * AsyncEventCenter.cpp
 *
 *  Created on: 2014年6月21日
 *      Author: jarek
 */

#include <Delay/DelayCenter.h>
#include <Util/Util.h>
#include <Log/LogMacro.h>
#include <typeinfo>



DelayCenter::DelayCenter()
{
    m_masterQueueIndex = 0;
    m_slaveQueueIndex = 1;
}

void DelayCenter::PushDelayFunction(DelayFunction* function)
{
    ASSERT_RETURN_VOID(function != NULL);

    ThreadMutexGuard lock(m_mutext);

    m_functionQueue[m_slaveQueueIndex].push(function);

    //TRACE("AsyncFunction <name:%s> push to center", typeid(*function).name());
}

void DelayCenter::InvokeDelayFunction()
{
    do
    {
        SwitchQueue();

        DelayFunctionQueue& masterQueue = m_functionQueue[m_masterQueueIndex];
        while (!masterQueue.empty())
        {
            DelayFunction* function = masterQueue.front();
            masterQueue.pop();

            ASSERT_CONTINUE(function != NULL);

            // TRACE("AsyncFunction <name:%s> invoke", typeid(*function).name());

            function->Invoke();

            SAFE_DELETE(function);
        }
    } while (!m_functionQueue[m_slaveQueueIndex].empty());
}

void DelayCenter::SwitchQueue()
{
    ThreadMutexGuard lock(m_mutext);

    int tempIndex = m_masterQueueIndex;
    m_masterQueueIndex = m_slaveQueueIndex;
    m_slaveQueueIndex = tempIndex;
}

