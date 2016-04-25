/*
*
*   FIFO Buffer Edit by kcinz 2012/2/6
*
*/
#pragma once

#include "Util/Util.h"
#include "Util/ErrorMessage.h"

#pragma pack(1)
struct FifoHead
{
    int                 beginNonius;                    // 起始游标
    int                 endNonius;                      // 终点游标    
    int                 contentLen;                     // 标识内容的长度
    int                 bufferLen;                      // 标识buffer的长度
};

struct FifoInfo
{
    FifoHead*           headInfo;                       // head信息
    char*               buffer;                         // 存储内容的Buffer
};
#pragma pack()

class FifoBuffer : public ErrorMessage
{
public:
    static int          GetHeadSize()
    {
        return sizeof(FifoHead);
    }

public:
                        FifoBuffer(char* buffer, int bufferLen);
                        FifoBuffer(int bufferLen);
                        ~FifoBuffer();
    
    int                 GetDataLen();                   /*get data buffer length*/
    int                 GetFreeLen();                   /*get free buffer length*/
    void                MoveNonius(int& nonius, int moveLen);       /*move Nonius*/

    bool                InsertBuffer(char* inBuffer, int inLen);
    void                GetBufferFromBegin(char* outBuffer, int& outLen, int outBufferLen);
    int                 DeleteDataInBuffer(int deleteLen);

    void                PrintfInfo();

private:
    char*               selfBuffer;                     // 自带BUFFER
    FifoInfo            fifoInfo;                       // FIFO的buffer信息
};

