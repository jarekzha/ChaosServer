#pragma once

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <Log/LogType.h>
#include <Log/LogGlobalParam.h>
#include <Time/Time.h>


#ifdef _DEBUG
#define PRINTF  printf
#else
#define PRINTF( _s_, ... )
#endif

#ifdef _DEBUG
#define TRACE(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        PRINTF("\033[1;33;40m%s[%s]%s......[%s:%s():%d]\033[0m\n", \
                   LogGlobalParam::Instance().LogTag.c_str(),\
                   Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
    }while(0)
#else
#define TRACE( _s_, ... )
#endif

#define TMP_LOG_LEN     1024

#define INIT_LOG_RUN(path, name)  LogRun::Instance().Initialize((char*)path, (char*)name)

#define LOG_RUN(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        LogRun::Instance().WriteLogToBuffer(_log_, strnlen(_log_, sizeof(_log_)));\
        PRINTF("\033[0;32;40m%s[%s]%s......[%s:%s():%d]\033[0m\n", \
                    LogGlobalParam::Instance().LogTag.c_str(),\
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        fflush(stdout); \
    }while(0)

#define INIT_LOG_ERR(path, name)  LogErr::Instance().Initialize((char*)path, (char*)name)
    
#define LOG_ERR(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        LogErr::Instance().WriteLogToBuffer(_log_, strnlen(_log_, sizeof(_log_)));\
        PRINTF("\033[0;31;40m%s[%s]%s......[%s:%s():%d]\033[0m\n", \
                    LogGlobalParam::Instance().LogTag.c_str(),\
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        fflush(stdout); \
    }while(0)

#define INIT_LOG_ANALYSIS(path, name)  LogAnalysis::Instance().Initialize((char*)path, (char*)name)

#define LOG_ANALYSIS(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        LogAnalysis::Instance().WriteLogToBuffer(_log_, strnlen(_log_, sizeof(_log_)));\
        PRINTF("%s[%s]%s......[%s:%s():%d]\n", \
                    LogGlobalParam::Instance().LogTag.c_str(),\
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        fflush(stdout); \
    }while(0)


#define INIT_LOG_TRACE(path, name)  LogTrace::Instance().Initialize((char*)path, (char*)name)

#define LOG_TRACE(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        LogTrace::Instance().WriteLogToBuffer(_log_, strnlen(_log_, sizeof(_log_)));\
        PRINTF("\033[1;33;40m%s[%s]%s......[%s:%s():%d]\033[0m\n", \
                    LogGlobalParam::Instance().LogTag.c_str(),\
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        fflush(stdout); \
    }while(0)

#define INIT_LOG_WARN(path, name)  LogWarn::Instance().Initialize((char*)path, (char*)name)

#define LOG_WARN(_s_, ...) do{\
        char _msg_[TMP_LOG_LEN];\
        char _log_[TMP_LOG_LEN];\
        snprintf(_msg_, sizeof(_msg_)-1, _s_, ##__VA_ARGS__);\
        _msg_[TMP_LOG_LEN-1] = 0;\
        snprintf(_log_, sizeof(_log_)-1, "[%s]%s......[%s:%s():%d]\n", \
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        _log_[TMP_LOG_LEN-1] = 0;\
        LogWarn::Instance().WriteLogToBuffer(_log_, strnlen(_log_, sizeof(_log_)));\
        PRINTF("\033[0;31;40m%s[%s]%s......[%s:%s():%d]\033[0m\n", \
                    LogGlobalParam::Instance().LogTag.c_str(),\
                    Time::CurDateTimeStr(), _msg_, __FILE__, __FUNCTION__, __LINE__);\
        fflush(stdout); \
    }while(0)


#define INIT_LOG_PATH_FILE(dirPath, runName, errName) do{\
        if (!INIT_LOG_RUN(dirPath, runName))\
        {\
            return -1;\
        }\
        if (!INIT_LOG_ERR(dirPath, errName))\
        {\
            return -1;\
        }\
    }while(0)

#define CLOSE_LOG()  LogRun::Instance().CloseLog();\
    LogErr::Instance().CloseLog();


