#pragma once

#include <sys/epoll.h>
#include "Util/Util.h"
#include "Util/ErrorMessage.h"

class Epoll : public ErrorMessage
{
private:
    int                     maxFd;
    int                     epollFd;
    struct epoll_event*     events;             

public:
                            Epoll();
                            ~Epoll();

    bool                    Initialize(int fdCount);

    void                    Release();

    int                     Wait(int msTime = 10);

    bool                    Add(int fd, int event = EPOLLIN);

    bool                    Mod(int fd, int event = EPOLLOUT);

    bool                    Del(int fd);

    bool                    DelByFdIndex(int fdIndex);

    int                     GetEventFd(int fdIndex);

    bool                    IsReadable(int fdIndex);

    bool                    IsWritable(int fdIndex);
};

