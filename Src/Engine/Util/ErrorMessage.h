/*
 * ErrorMessage.h
 *
 *  Created on: 2014年7月1日
 *      Author: jarek
 */

#pragma once
#include <stdio.h>
#include <string>
#include <assert.h>
#include <stdarg.h>

#define COMMON_ERROR_MSG          200

#define COMMON_ERROR(_s_, ...)  \
        LogErrorMessage(__FILE__, __FUNCTION__, __LINE__, _s_, ##__VA_ARGS__)

#define GET_ERROR_MSG(p)  p->GetErrorMessage()
#define GET_ERROR_MSG_REFE(p)  p.GetErrorMessage()


class ErrorMessage
{
public:
    const char*         GetErrorMessage()
    {
        return errorMessage;
    }

protected:
    void                LogErrorMessage(const char* file,
                                        const char* function,
                                        int line,
                                        const char* format, ...)
    {
        if (NULL == file || NULL == function || NULL == format)
        {
            assert(false);
            return;
        }

        va_list valist;
        va_start(valist, format);

        char tempMessage[COMMON_ERROR_MSG];
        snprintf(tempMessage, sizeof(tempMessage)-1, format, valist);
        tempMessage[COMMON_ERROR_MSG - 1] = 0;
        snprintf(errorMessage, sizeof(errorMessage)-1, "%s......[%s:%s():%d]",
                tempMessage, file, function, line);

        va_end(valist);
    }

private:
    char                errorMessage[COMMON_ERROR_MSG];
};


