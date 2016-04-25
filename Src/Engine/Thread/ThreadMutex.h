///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  线程互斥类
//          edit by kcinz
//

#pragma once

#include <pthread.h>
#include <Thread/SyncObject.h>

class ThreadMutex : public SyncObject
{
public:
                        ThreadMutex();                       
    virtual             ~ThreadMutex();

// SyncObject
public:
    virtual int         Lock()      { return pthread_mutex_lock(&mutexHandle); }
    virtual int         Unlock()    { return pthread_mutex_unlock(&mutexHandle); }
    virtual int         TryLock()   { return pthread_mutex_trylock(&mutexHandle); }

public:
    pthread_mutex_t*    GetMutex()  { return &mutexHandle; }

private:
    bool                isInitialized;
    pthread_mutex_t     mutexHandle;
    pthread_mutexattr_t mutexAttribute;
};

class ThreadMutexGuard
{
public:
    explicit            ThreadMutexGuard(ThreadMutex& mutex) : mutex(mutex)
    {
        mutex.Lock();
    }

                        ~ThreadMutexGuard()
    {
        mutex.Unlock();
    }
private:
    ThreadMutex&       mutex;
};

#define ThreadMutexGuard(x) static_assert(false, "missing mutex guard var name")

