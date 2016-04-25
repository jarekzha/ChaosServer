/*
*  多线程日志类，主线程负责向buffer写日志，工作线程负责读取内存日志并写入磁盘，以及按照大小切换文件
*  edit by kcinz 2012/02/10
*/

#pragma once

#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <Util/FIFOBuffer.h>
#include <Thread/ThreadMutex.h>

void PrintfNone(char* format, ...);

#define     LOG_READ_BUFFER        1024
#define     LOG_FILE_NAME_LEN      30

class LogWriter
{    
public:
                        LogWriter();
                        ~LogWriter();

    static void*        ThreadFunc(void* arg);

    /*
    *   logPath     - only path for log
    *   fileName    - log's file name
    *   cacheSize   - log's cache size for FIFO buffer
    *   maxFileSize - log file Size
    *   maxLogNum   - max Log file number
    *
    */
    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152, 
                                             int maxFileSize = 1073741824, int maxLogNum = 10);

    void                WriteLogToBuffer(char* log, int logLen);

    void                WriteLogToFile();    

    void                ShiftFile();

    pthread_t           GetThreadID()   { return threadID; }

    void                CloseLog();

    bool                GetRunFlag()    { return runFlag; }
    
private:
    FifoBuffer*         logBuffer;
    pthread_t           threadID;
    char                logPath[PATH_MAX];
    char                readBuffer[LOG_READ_BUFFER];
    FILE*               logFile;
    int                 maxFileSize;
    char                fileName[LOG_FILE_NAME_LEN];
    char                fullPath[PATH_MAX];
    int                 maxLogNum;
    volatile bool       runFlag;

    ThreadMutex         logMutex;
};

