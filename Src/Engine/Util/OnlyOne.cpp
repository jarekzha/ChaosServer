/*
 * OnlyOne.cpp
 *
 *  Created on: 2014年7月4日
 *      Author: kcinz
 */

#include "OnlyOne.h"

OnlyOne::OnlyOne(const char* appname, int procID)
{
    m_appname = (char*) appname;
    m_appProcID = procID;
}

OnlyOne::~OnlyOne()
{
    m_appname = NULL;
    m_appProcID = 0;
}

bool OnlyOne::TestOnlyOne()
{
    if (NULL == m_lockfile || 0 == m_lockfile[0])
        return false;

    int fd = open(m_lockfile, O_RDWR | O_CREAT, S_IRWXU);
    if (fd < 0)
    {
        return false;
    }

    int lockret = lockf(fd, F_TEST, 0);
    if (lockret < 0)
    {
        close(fd);
        return false;
    }

    close(fd);
    return true;
}

bool OnlyOne::GetOnlyOne()
{
    if (NULL == m_lockfile || 0 == m_lockfile[0])
        return false;

    int fd = open(m_lockfile, O_RDWR | O_CREAT, S_IRWXU);
    if (fd < 0)
    {
        return false;
    }

    int lockret = lockf(fd, F_TLOCK, 0);
    if (lockret < 0)
    {
        close(fd);
        return false;
    }

    char pidstr[10] =
    { 0 };
    snprintf(pidstr, sizeof(pidstr), "%u", getpid());
    write(fd, pidstr, strlen(pidstr));

    // close(fd); <--- 万万不可做此操作，否则失效
    return true;
}

bool OnlyOne::OnlyOneDaemon(int nochdir, int noclose)
{
    snprintf(m_lockfile, sizeof(m_lockfile), "/tmp/%s_%d.pid", m_appname, m_appProcID);

    if (!TestOnlyOne())
    {
        printf("Warning: %s has been running! please check with 'ps' command\n", m_appname);
        return false;
    }

    if (daemon(nochdir, noclose) < 0)
    {
        printf("Error: %s daemon failed!\n", m_appname);
        return false;
    }

    if (!GetOnlyOne())
    {
        printf("Error: %s, get only one process failed!", m_appname);
        assert(false);
        return false;
    }

    return true;
}

