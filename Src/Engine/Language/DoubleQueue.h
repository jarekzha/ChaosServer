/*
 * SwitchList.h
 *
 *  Created on: 2015年11月14日
 *      Author: jarek
 */
#pragma once
#include <Thread/ThreadMutex.h>
#include <queue>

/**
 * 双队列，用于处理两个线程一读一写的同步问题
 */
template <typename T>
class DoubleQueue
{
public:
    DoubleQueue();
    virtual             ~DoubleQueue();

public:
    void                Push(T& item);

    T&                  Pop();

    bool                Empty();

    void                Switch();

private:
    typedef std::queue<T> QueueType;

private:
    ThreadMutex         m_mutext;
    QueueType           m_doubleQueue[2];

    int                 m_read;     // 读队列下标
    int                 m_write;    // 写队列下标
};

// -----------------------------------------------------
template <typename T>
DoubleQueue<T>::DoubleQueue()
{
    m_read = 0;
    m_write = 1;
}

template <typename T>
DoubleQueue<T>::~DoubleQueue()
{
    // NULL
}

template <typename T>
void DoubleQueue<T>::Push(T& item)
{
    ThreadMutexGuard lock(m_mutext);
    m_doubleQueue[m_write].push(item);
}

template <typename T>
T& DoubleQueue<T>::Pop()
{
    T& item = m_doubleQueue[m_read].front();
    m_doubleQueue[m_read].pop();
    return item;
}

template <typename T>
bool DoubleQueue<T>::Empty()
{
    return m_doubleQueue[m_read].empty();
}

template <typename T>
void DoubleQueue<T>::Switch()
{
    ThreadMutexGuard lock(m_mutext);
    int tmp = m_read;
    m_read = m_write;
    m_write = tmp;
}








