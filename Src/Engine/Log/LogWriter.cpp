#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <dirent.h>
#include <Time/Time.h>
#include <Log/LogWriter.h>

void PrintfNone(char* format, ...)
{
    return;
}

LogWriter::LogWriter()
{
    logBuffer       = NULL;
    threadID        = 0;
    memset(logPath, 0, sizeof(logPath));
    memset(readBuffer, 0, sizeof(readBuffer));
    logFile         = NULL;
    maxFileSize     = 0;
    memset(fileName, 0, sizeof(fileName));
    maxLogNum       = 0;
    runFlag         = true;
}

LogWriter::~LogWriter()
{
    if (logFile)
    {
        fclose(logFile);
    }
    
    SAFE_DELETE(logBuffer);
    threadID        = 0;
    memset(logPath, 0, sizeof(logPath));
    memset(readBuffer, 0, sizeof(readBuffer));
    maxFileSize     = 0;
    memset(fileName, 0, sizeof(fileName));
    maxLogNum       = 0;
    runFlag         = false;
}

void* LogWriter::ThreadFunc(void* arg)
{
    LogWriter* myLog = (LogWriter*)arg;

    while(myLog->GetRunFlag())
    {
        myLog->WriteLogToFile();

        Time::MsSleep(1);
    }

    return NULL;
}

bool LogWriter::Initialize(char* logPath, char* fileName, int cacheSize, int maxFileSize, int maxLogNum)
{
    strncpy(this->logPath, logPath, sizeof(this->logPath));
    strncpy(this->fileName, fileName, sizeof(this->fileName));
    
    snprintf(this->fullPath, sizeof(this->fullPath)-1, "%s/%s.log", this->logPath, this->fileName);
    this->fullPath[sizeof(this->fullPath)-1] = 0;

    if (opendir(this->logPath) == NULL)
    {
        if (mkdir(this->logPath, 0777) < 0)
        {
            printf("error: %s\n", strerror(errno));
            assert(false);
            return false;
        }
    }

    logFile = fopen(this->fullPath, "a+");
    if (NULL == logFile)
    {
        printf("error %s: %s\n", this->fullPath, strerror(errno));
        assert(false);
        return false;
    }

    this->maxLogNum = maxLogNum;

    logBuffer = new FifoBuffer(cacheSize);

    this->maxFileSize = maxFileSize;

    pthread_create(&threadID, NULL, ThreadFunc, (void*)this);

    return true;
}

void LogWriter::WriteLogToBuffer(char* log, int logLen)
{
    if (NULL == logBuffer)
    {
        return;
    }

    ThreadMutexGuard lock(logMutex);
    logBuffer->InsertBuffer(log, logLen);
}

void LogWriter::WriteLogToFile()
{
    if (NULL == logFile)
    {
        // some wrong in shiftFile()
        assert(false);
        return;
    }
    
    int readLen = 0;

    logBuffer->GetBufferFromBegin(readBuffer, readLen, sizeof(readBuffer));

    if (0 == readLen)
    {
        return;
    }

    
    {
        ThreadMutexGuard lock(logMutex);
        logBuffer->DeleteDataInBuffer(readLen);
    }

    fwrite(readBuffer, sizeof(char), readLen, logFile);
    fflush(logFile);

    ShiftFile();
}

void LogWriter::ShiftFile()
{
    struct stat fileStat;

    if (stat(fullPath, &fileStat) < 0)
    {
        printf("error:%s\n", strerror(errno));
        return;
    }

    if (fileStat.st_size < maxFileSize)
    {
        return;
    }

    fclose(logFile);

    char oldFileName[PATH_MAX];
    char newFileName[PATH_MAX];
    for (int id = maxLogNum - 2; id >=0; id--)
    {
        if (0 == id)
        {
            snprintf(oldFileName, sizeof(oldFileName)-1, "%s/%s.log", logPath, fileName);
        }
        else
        {
            snprintf(oldFileName, sizeof(oldFileName)-1, "%s/%s%d.log", logPath, fileName, id);
        }

        if (access(oldFileName, F_OK) == 0)
        {
            snprintf(newFileName, sizeof(newFileName)-1, "%s/%s%d.log", logPath, fileName, id+1);
            if (rename(oldFileName, newFileName) < 0)
            {
                printf("error: %s\n", strerror(errno));
                break;
            }
        }
    }

    logFile = fopen(this->fullPath, "a+");
    if (NULL == logFile)
    {
        printf("error: %s\n", strerror(errno));
        return;
    }
}

void LogWriter::CloseLog()      
{ 
    runFlag = false; 
    pthread_join(threadID, NULL);
}

