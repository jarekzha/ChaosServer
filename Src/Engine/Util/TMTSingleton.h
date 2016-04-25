///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  线程安全的单键模版
//          edit by kcinz
//

#pragma once

#include "Thread/ThreadMutex.h"

template<typename T>
class TMTSingleton
{
protected:
                        TMTSingleton()     {}
    virtual             ~TMTSingleton()    {}

private:
                        TMTSingleton(const TMTSingleton&);
    TMTSingleton&       operator=(const TMTSingleton&);

public:
    static T&           Instance()
    {
        if (false == _flag)
        {
            ThreadMutexGuard lock(_mutex);
            if (false == _flag)
            {
                // T* volatile tmp = new T;    // 同_flag意义相同
                _instance = new T;
                _flag = true;               // 避免编译器优化operator new和placement new的操作顺序，导致获取一个未初始化完成的指针
            }
        }

        return *_instance;
    }

private:
    static ThreadMutex _mutex;
    static T* volatile  _instance;          // volatile 防止RISC机器的编译优化合格两个if，破坏double-check，从而导致内存泄漏
    static bool volatile _flag;             // 是否初始化成功的标志位
    //bool              _destory;           // 参考Phoenix Singleton模式下的Dead Reference
};

template<typename T>
T* volatile TMTSingleton<T>::_instance = NULL;

template<typename T>
ThreadMutex TMTSingleton<T>::_mutex;

template<typename T>
bool volatile TMTSingleton<T>::_flag = false;


