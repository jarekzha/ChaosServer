//
//  XXTEA.cpp
//
//  Created on 2014/08/18.
//  Copyright (c) 2014. All rights reserved.
//

/*
 * 说明：加密格式 原始数据长度(int) + 实际加密数据(binary)
 *
 * 加密：在传入保存加密数据buf头部四字节保存原始数据长度，四字节之后保存真实加密数据
 *
 * 解密：取出头部四字节真实数据长度，从四字节之后开始解密
 *
 * */

#include "XXTEA.h"
#define     MX      (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (k[(p&3)^e] ^ z)))
#define     XXTEA_ALIGNMENT_BYTE                 	(4)

using namespace std;

XXTEA*	XXTEA::_inst = NULL;

XXTEA::XXTEA()
{

}

XXTEA::~XXTEA()
{

}

XXTEA*	XXTEA::instance()
{
	//multit-hread need add double cheak
	if (NULL == _inst)
	{
		_inst = new XXTEA();
	}
	
	return _inst;
}

size_t XXTEA::XXTEAGetBufferLength(size_t nLength, bool isEncode)
{
    if (isEncode) 
	{
        size_t nSize = nLength / XXTEA_ALIGNMENT_BYTE + (nLength % XXTEA_ALIGNMENT_BYTE  ? 1 : 0);
        return (nSize * XXTEA_ALIGNMENT_BYTE * 2 + 1);
    }
    else 
	{
        return (nLength / 2 + 1);
    }
}

bool XXTEA::XXTEAEncode(char* lpDstBuffer, int* encodeLen, const char* lpSrcBuffer, size_t nLength, const XXTEA_TYPE* lpKey)
{
    long ret = 1;
    if (nLength > XXTEA_ALIGNMENT_BYTE && lpDstBuffer && lpSrcBuffer) 
	{
        size_t nSize = nLength / XXTEA_ALIGNMENT_BYTE + (nLength % XXTEA_ALIGNMENT_BYTE  ? 1 : 0);
		if (*encodeLen < (int)(nSize*XXTEA_ALIGNMENT_BYTE + sizeof(int)))
		{
		    *encodeLen = nSize*XXTEA_ALIGNMENT_BYTE + sizeof(int);
		    return false;
		}

        *encodeLen = sizeof(int) + nSize*XXTEA_ALIGNMENT_BYTE;

		char* data = lpDstBuffer;
		*((int *)data) = nLength;       //buffer头部四字节存储未加密数据真实长度

		data += sizeof(int);
		memcpy(data, lpSrcBuffer, nLength);
		ret = btea((XXTEA_TYPE*)data, nSize, lpKey);
    }
    return (ret == 0);
}

bool XXTEA::XXTEADecode(char* lpDstBuffer, int* decodeLen, const char* lpSrcBuffer, size_t nLength, const XXTEA_TYPE* lpKey)
{
    long ret = 1;
    if (nLength > XXTEA_ALIGNMENT_BYTE * 2 && lpDstBuffer && lpSrcBuffer) 
	{
        int serializeLen = *((int *)lpSrcBuffer);
        if (*decodeLen < (int)nLength)
        {
            return false;
        }

        *decodeLen = serializeLen;
        size_t nSize = serializeLen / XXTEA_ALIGNMENT_BYTE + (serializeLen % XXTEA_ALIGNMENT_BYTE  ? 1 : 0);

        const char*data = lpSrcBuffer + sizeof(int);
        memcpy(lpDstBuffer, data, (nLength - sizeof(int)));
        ret = btea((XXTEA_TYPE*)lpDstBuffer, -nSize, lpKey);
    }
    return (ret == 0);
}

long XXTEA::btea(XXTEA_TYPE* v, long n, const XXTEA_TYPE* k) 
{
    XXTEA_TYPE z, y;
	unsigned long sum = 0, e, DELTA = 0x9e3779b9;
	long p, q;
    if (n > 1) 
	{// Coding Part 
		z = v[n-1];
        q = 6 + 52 / n;
        while (q-- > 0) 
		{
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p = 0; p < n-1; ++p) y = v[p+1], z = v[p] += MX;
            y = v[0];
            z = v[n-1] += MX;
        }
        return 0;
    } 
	else if (n < -1) 
	{// Decoding Part 
		y = v[0];
        n = -n;
        q = 6 + 52 / n;
        sum = q * DELTA ;
        while (sum != 0) 
		{
            e = (sum >> 2) & 3;
            for (p = n-1; p > 0; --p) z = v[p-1], y = v[p] -= MX;
            z = v[n-1];
            y = v[0] -= MX;
            sum -= DELTA;
        }
        return 0;
    }
    return 1;
}

void XXTEA::StringToHex(const char* surBuffer, int length, char* dstBuffer)
{
    for (int i = 0; i < length; i++)
    {
        sprintf(&dstBuffer[2*i], "%02X", (unsigned char)surBuffer[i]);
    }
    dstBuffer[2 * length] = '\0';
}

int XXTEA::HexToString(const char* surBuffer,  char* dstBuffer)
{
    size_t length = strlen((char*)surBuffer);
    for(size_t i = 0; i < length/2; i++)
    {
        int temp = 0;
        sscanf(&surBuffer[2 * i], "%02X", &temp);
        dstBuffer[i] = (unsigned char)temp;
    }
    dstBuffer[length/2] = '\0';
	return (int)strlen((char*)dstBuffer);
}

