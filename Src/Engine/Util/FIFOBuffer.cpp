#include "Common.h"
#include <Util/FIFOBuffer.h>

FifoBuffer::FifoBuffer(char* buffer, int bufferLen)
{   
    memset(buffer, 0, bufferLen);
    
    fifoInfo.headInfo = (FifoHead*)buffer;
    
    fifoInfo.headInfo->bufferLen = bufferLen - sizeof(FifoHead);
    fifoInfo.buffer = buffer + sizeof(FifoHead);
    
    selfBuffer = NULL;
}

FifoBuffer::FifoBuffer(int bufferLen)
{
    selfBuffer = new char[bufferLen];

    memset(selfBuffer, 0, bufferLen);

    fifoInfo.headInfo = (FifoHead*)selfBuffer;
    
    fifoInfo.headInfo->bufferLen = bufferLen - sizeof(FifoHead);
    fifoInfo.buffer = selfBuffer + sizeof(FifoHead);
}

FifoBuffer::~FifoBuffer()
{
    fifoInfo.headInfo   = NULL;
    fifoInfo.buffer     = NULL;

    SAFE_DELETE_ARRAY(selfBuffer);
}

int FifoBuffer::GetDataLen()
{
    /* 用注释中的方法无法区分满Buffer和空Buffer
    //head --- begin ***  end  ---  tail
    if (beginNonius <= endNonius)
    {
        return endNonius - beginNonius;
    }
    //head *** end  ---  begin  ***  tail
    else
    {
        return bufferLen - (beginNonius - endNonius);
    }
    */

    return fifoInfo.headInfo->contentLen;
}

int FifoBuffer::GetFreeLen()
{
    return fifoInfo.headInfo->bufferLen - GetDataLen();
}

void FifoBuffer::MoveNonius(int& nonius, int moveLen)
{
    nonius += moveLen;
    nonius = (nonius > fifoInfo.headInfo->bufferLen) ? nonius-fifoInfo.headInfo->bufferLen : nonius;
}

bool FifoBuffer::InsertBuffer(char* inBuffer, int inLen)
{
    /*不能放入比buffer大的内容*/
    if (inLen > GetFreeLen())
    {
        COMMON_ERROR("[No enough buffer to insert!]");
        return false;
    }

    char* bufferHead    = fifoInfo.buffer;
    char* bufferEnd     = fifoInfo.buffer + fifoInfo.headInfo->endNonius;

    /*head --- begin ***  end  --- tail*/
    if (fifoInfo.headInfo->beginNonius <= fifoInfo.headInfo->endNonius)
    {
        int endToTail = fifoInfo.headInfo->bufferLen - fifoInfo.headInfo->endNonius;/*end --- tail*/

        if (inLen < endToTail)
        {
            memcpy(bufferEnd, inBuffer, inLen);
        }
        else
        {
            /*  judged in function begin ---- !!!
            if (inLen < getFreeLen())
            {
                LOG_RUN("No enough buffer to insert!");
                assert(false);
                return false;
            }*/

            memcpy(bufferEnd, inBuffer, endToTail);
            memcpy(bufferHead, inBuffer+endToTail, inLen-endToTail);
        }
    }
    /*head *** end  ---  begin  *** tail*/
    else
    {
        /*  judged in function begin ---- !!!
        if (inLen < getFreeLen())
        {
            LOG_RUN("No enough buffer to insert!");
            assert(false);
            return false;
        }*/

        memcpy(bufferEnd, inBuffer, inLen);
    }

    MoveNonius(fifoInfo.headInfo->endNonius, inLen);
    fifoInfo.headInfo->contentLen += inLen;

    return true;
}

void FifoBuffer::GetBufferFromBegin(char* outBuffer, int& outLen, int outBufferLen)
{
    outLen = GetDataLen();

    if (0 == outLen)
    {
        // no data
        return;
    }

    /*无法取出比outBuffer大的内容*/
    if (outBufferLen < GetDataLen())
    {
        outLen = outBufferLen;
    }

    char* bufferBegin       = fifoInfo.buffer + fifoInfo.headInfo->beginNonius;
    char* bufferHead        = fifoInfo.buffer;
    /*head --- begin ****  end  --- tail*/
    if (fifoInfo.headInfo->beginNonius <= fifoInfo.headInfo->endNonius)
    {
        memcpy(outBuffer, bufferBegin, outLen);
    }
    /*head **** end  ---  begin **** tail*/
    else
    {
        int beginToTail     = fifoInfo.headInfo->bufferLen - fifoInfo.headInfo->beginNonius;

        if (outLen <= beginToTail)
        {
            memcpy(outBuffer, bufferBegin, outLen);
        }
        else
        {
            memcpy(outBuffer, bufferBegin, beginToTail);
            memcpy(outBuffer+beginToTail, bufferHead, outLen-beginToTail);
        }
    }

    // 此处只获取不移动游标删除，删除操作统一由deleteDataInBuffer完成
    //moveNonius(beginNonius, outLen);
}

int FifoBuffer::DeleteDataInBuffer(int deleteLen)
{
    int realDelete = (deleteLen <= GetDataLen()) ? deleteLen : GetDataLen();

    MoveNonius(fifoInfo.headInfo->beginNonius, realDelete);

    fifoInfo.headInfo->contentLen -= realDelete;

    return realDelete;
}

void FifoBuffer::PrintfInfo()
{
    printf("\tfree Len = %d\n",       GetFreeLen());
    printf("\tdata Len = %d\n",       GetDataLen());
    printf("\tbegin nonius = %d\n",   fifoInfo.headInfo->beginNonius);
    printf("\tend nonius = %d\n",     fifoInfo.headInfo->endNonius);

    printf("buffer content:\n[");
    for (int i = 0; i < fifoInfo.headInfo->bufferLen; i++)
    {
        printf("%c", fifoInfo.buffer[i]);
    }
    printf("]\n[");

    for (int i = 0; i < fifoInfo.headInfo->bufferLen; i++)
    {
        if (i == fifoInfo.headInfo->beginNonius)
        {
            printf("b");
        }
        else if (i == fifoInfo.headInfo->endNonius)
        {
            printf("e");
        }
        else
        {
            printf(" ");
        }
    }

    printf("]\n");
}


