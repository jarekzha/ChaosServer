#include "Thread/ThreadMutex.h"

ThreadMutex::ThreadMutex()
{
    if (0 != pthread_mutexattr_init(&mutexAttribute))
    {
        isInitialized = false;
        return;
    }

    pthread_mutexattr_settype(&mutexAttribute, PTHREAD_MUTEX_RECURSIVE_NP); //设置锁的属性为可递归

    if (0 != pthread_mutex_init(&mutexHandle, &mutexAttribute))
    {
        pthread_mutexattr_destroy(&mutexAttribute);
        isInitialized = false;
        return;
    }

    isInitialized = true;
}

ThreadMutex::~ThreadMutex()
{
    if (isInitialized)
    {
        pthread_mutexattr_destroy(&mutexAttribute);
        pthread_mutex_destroy(&mutexHandle);
        isInitialized = false;
    }
}

