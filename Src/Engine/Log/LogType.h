/*
*
*   
*
*/
#pragma once

#include "Log/LogWriter.h"
#include "Util/TMTSingleton.h"

class LogRun : public TMTSingleton<LogRun>
{
public:
                        LogRun()   {}
                        ~LogRun()  {}

    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152, 
                                             int maxFileSize = 1073741824, int maxLogNum = 10)
    {
        return logRun.Initialize(logPath, fileName, cacheSize, maxFileSize, maxLogNum);
    }
    
    void                WriteLogToBuffer(char* log, int logLen)
    {
        logRun.WriteLogToBuffer(log, logLen);
    }

    void                CloseLog()
    {
        logRun.CloseLog();
    }

private:                            
    LogWriter           logRun;
};

class LogErr : public TMTSingleton<LogErr>
{
public:
                        LogErr()   {}
                        ~LogErr()  {}
                        
    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152, 
                                             int maxFileSize = 1073741824, int maxLogNum = 10)
    {
        return logErr.Initialize(logPath, fileName, cacheSize, maxFileSize, maxLogNum);
    }
    
    void                WriteLogToBuffer(char* log, int logLen)
    {
        logErr.WriteLogToBuffer(log, logLen);
    }

    void                CloseLog()
    {
        logErr.CloseLog();
    }
    
private:    
    LogWriter           logErr;
};

class LogAnalysis : public TMTSingleton<LogAnalysis>
{
public:
                        LogAnalysis()   {}
                        ~LogAnalysis()  {}

    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152,
                                             int maxFileSize = 1073741824, int maxLogNum = 10)
    {
        return logAnalysis.Initialize(logPath, fileName, cacheSize, maxFileSize, maxLogNum);
    }

    void                WriteLogToBuffer(char* log, int logLen)
    {
        logAnalysis.WriteLogToBuffer(log, logLen);
    }

    void                CloseLog()
    {
        logAnalysis.CloseLog();
    }

private:
    LogWriter           logAnalysis;
};

class LogTrace : public TMTSingleton<LogTrace>
{
public:
                        LogTrace()   {}
                        ~LogTrace()  {}

    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152,
                                             int maxFileSize = 1073741824, int maxLogNum = 10)
    {
        return logTrace.Initialize(logPath, fileName, cacheSize, maxFileSize, maxLogNum);
    }

    void                WriteLogToBuffer(char* log, int logLen)
    {
        logTrace.WriteLogToBuffer(log, logLen);
    }

    void                CloseLog()
    {
        logTrace.CloseLog();
    }

private:
    LogWriter           logTrace;
};

class LogWarn : public TMTSingleton<LogWarn>
{
public:
                        LogWarn()   {}
                        ~LogWarn()  {}

    bool                Initialize(char* logPath, char* fileName, int cacheSize = 2097152,
                                             int maxFileSize = 1073741824, int maxLogNum = 10)
    {
        return logWarn.Initialize(logPath, fileName, cacheSize, maxFileSize, maxLogNum);
    }

    void                WriteLogToBuffer(char* log, int logLen)
    {
        logWarn.WriteLogToBuffer(log, logLen);
    }

    void                CloseLog()
    {
        logWarn.CloseLog();
    }

private:
    LogWriter           logWarn;
};



