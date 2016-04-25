#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "Util/TMTSingleton.h"

class OnlyOne: public TMTSingleton<OnlyOne>
{
public:
    OnlyOne(const char* appname, int procID);
    ~OnlyOne();

    bool TestOnlyOne();
    bool GetOnlyOne();
    bool OnlyOneDaemon(int nochdir, int noclose);

private:
    char m_lockfile[PATH_MAX];
    char* m_appname;
    int m_appProcID;
};

