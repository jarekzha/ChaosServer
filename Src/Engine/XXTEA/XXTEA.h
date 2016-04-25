//
//  XXTEA.h
//
//  Created on 2014/08/18.
//  Copyright (c) 2014. All rights reserved.
//

#ifndef XXTEA_h
#define XXTEA_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef XXTEA_TYPE
#define XXTEA_TYPE uint32_t         // 一种32位长的数据类型，因uint32_t在32bit和64bit系统中都是32位的，故直接用uint32_t
#endif

class XXTEA
{
public:
	static XXTEA*	instance();
	
public:
    /**
     *	@brief     获得输出缓冲区的长度
     *
     *	@param 	nLength 	输入缓冲区长度
     *	@param 	isEncode 	TRUE 即为加密，FALSE即为解密
     *
     *	@return	返回所需长度
     */
	size_t XXTEAGetBufferLength(size_t nLength, bool isEncode);
    
    /**
     *	@brief	对数据流进行加密
     *
     *	@param 	lpDstBuffer 	目标缓冲区
     *	@param  encodeLen       传入目标缓冲区大小，传出加密数据大小
     *	@param 	lpSrcBuffer 	源缓冲区
     *	@param 	nLength 		源缓冲区长度
     *	@param 	lpKey 			密钥--XXTEA_TYPE key[4]
     *
     *	@return	加密是否成功
     */
	bool XXTEAEncode(char* lpDstBuffer, int* encodeLen, const char* lpSrcBuffer, size_t nLength, const XXTEA_TYPE* lpKey);

    /**
     *	@brief	对数据流进行解密
     *
     *	@param 	lpDstBuffer 	目标缓冲区
	 *	@param	decodeLen		传入目标缓冲区大小，传出解密后数据长度
     *	@param 	lpSrcBuffer 	源缓冲区
     *	@param 	nLength 		源缓冲区长度
     *	@param 	lpKey 			密钥
     *
     *	@return	解密是否成功
     */
	bool XXTEADecode(char* lpDstBuffer, int* decodeLen, const char* lpSrcBuffer, size_t nLength, const XXTEA_TYPE* lpKey);
	
    /**
     *  @brief  正常字符串以16进制保存
     *
     *  @param  surBuffer   源缓冲区
     *  @param  length  源缓冲区长度
     *  @param  dstBuffer   目标缓冲区
     */
    void StringToHex(const char* surBuffer, int length, char* dstBuffer);

    /**
     *  @brief  16进制字符串解析成正常字符串
     *
     *  @param  surBuffer   源缓冲区
     *  @param  dstBuffer   目标缓冲区
     *
     *  @return 目标缓冲区的长度
     */
    int HexToString(const char* surBuffer,  char* dstBuffer);

private:
    /**
     *	@brief	XXTEA官方算法，此类即是对其进行封装，详见http://en.wikipedia.org/wiki/XXTEA
     *
     *	@param 	v 	加解密数据流
     *	@param 	n 	加解密长度，n > 1为加密，n < -1为解密
     *	@param 	k 	密钥      注意：形式参数k需和MX中的k对应
     *
     *	@return	返回0表示加解密成功，返回1表示失败
     */
	long btea(XXTEA_TYPE* v, long n, const XXTEA_TYPE* k);
	
public:
    XXTEA();
    ~XXTEA();	
	
private:
	static	XXTEA*	_inst;
};

#endif
