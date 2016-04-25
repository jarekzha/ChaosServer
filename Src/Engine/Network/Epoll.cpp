#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "Network/Epoll.h"

Epoll::Epoll()
{
    maxFd               = 0;
    epollFd             = -1;
    events              = NULL;
}

Epoll::~Epoll()
{
    Release();
}

bool Epoll::Initialize(int fdCount)
{
    maxFd = fdCount;
    epollFd = epoll_create(maxFd);

    if (epollFd < 0)
    {
        COMMON_ERROR("create epollFd Failed! <epollFd:%d>! error:%s", epollFd, strerror(errno));
        return false;
    }

    if (NULL == events)
    {
        events = new epoll_event[maxFd];
    }    

    return true;
}

void Epoll::Release()
{
    maxFd               = 0;

    if (epollFd >= 0)
    {
        close(epollFd);
        epollFd         = -1;
    }
    
    if (events)
    {
        delete[]    events;
        events          = NULL;
    }
}

int Epoll::Wait(int msTime)
{
    return epoll_wait(epollFd, events, maxFd, msTime);
}

bool Epoll::Add(int fd, int event)
{
    struct epoll_event              ev;

    ev.events           = event;
    ev.data.fd          = fd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev))
    {
        COMMON_ERROR("Epoll Add Filed!<fd:%d> <event:%d>", fd, event);
        return false;
    }

    return true;
}

bool Epoll::Mod(int fd, int event)
{
    struct epoll_event              ev;

    ev.events           = event;
    ev.data.fd          = fd;

    if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &ev))
    {
        COMMON_ERROR("Epoll Modify Filed!<fd:%d> <event:%d>", fd, event);
        return false;
    }

    return true;
}

bool Epoll::Del(int fd)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, NULL))
    {
        COMMON_ERROR("Epoll Delete Filed!<fd:%d> <error:%s>", fd, strerror(errno));
        return false;
    }

    return true;
}

bool Epoll::DelByFdIndex(int fdIndex)
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, GetEventFd(fdIndex), NULL))
    {
        COMMON_ERROR("Epoll Delete Filed!<fd:%d> <error:%s>", GetEventFd(fdIndex), strerror(errno));
        return false;
    }

    events[fdIndex].data.fd = -1;

    return true;
}

int Epoll::GetEventFd(int fdIndex)
{
    if (fdIndex < 0 || fdIndex >= maxFd)
    {
        COMMON_ERROR("fdIndex is Error!<fdIndex:%d>", fdIndex);
        return -1;
    }

    return events[fdIndex].data.fd;
}

bool Epoll::IsReadable(int fdIndex)
{
    if (fdIndex < 0 || fdIndex >= maxFd)
    {
        COMMON_ERROR("fdIndex is Error!<fdIndex:%d>", fdIndex);
        return false;
    }

    return (events[fdIndex].events & EPOLLIN);
}

bool Epoll::IsWritable(int fdIndex)
{
    if (fdIndex < 0 || fdIndex >= maxFd)
    {
        COMMON_ERROR("fdIndex is Error!<fdIndex:%d>", fdIndex);
        return false;
    }

    return (events[fdIndex].events & EPOLLOUT);
}

