/*
 * MessageXXTEAHelper.cpp
 *
 *  Created on: 2014年9月5日
 *      Author: silas
 */

#include "MessageXXTEAHelper.h"
#include <Engine/Util/FakeRandom.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Time/Time.h>

#define MAX_DECODEBUF_LEN       (2048)
#define TEA_KEY_UPDATE_POINT    (1800)  //半小时更新一次key
#define LRU_TIME_POINT          (7200)  //两小时触发一次lru
#define SESSION_EXPIRE_GAP      (43200) //淘汰12小时内未做过操作的

MessageXXTEAHelper::MessageXXTEAHelper()
    : teaKeyMap(), lruMapSet(), lastLRUTime(0)
    , encryptbuf(NULL), decryptbuf(NULL)
{
    defaultKey[0] = 0x12345678;
    defaultKey[1] = 0x734a67fc;
    defaultKey[2] = 0x3367a642;
    defaultKey[3] = 0x78432562;

    encryptbuf = new char[MAX_DECODEBUF_LEN];
    decryptbuf = new char[MAX_DECODEBUF_LEN];
}

MessageXXTEAHelper::~MessageXXTEAHelper()
{
    if (NULL != encryptbuf)
    {
        delete [] encryptbuf;
        encryptbuf = NULL;
    }

    if (NULL != decryptbuf)
    {
        delete [] decryptbuf;
        decryptbuf = NULL;
    }
}

bool MessageXXTEAHelper::Encrypt(unsigned sessionID, const char* orignalBuf, size_t orignalBufLen, std::string& encodeStr)
{
    encodeStr.clear();
    const XXTEA_TYPE* teaKey = GetKey(sessionID);
    if (NULL == teaKey)
    {
        teaKey = defaultKey;
    }

    int encodeLen = MAX_DECODEBUF_LEN;
    if (!XXTEA::instance()->XXTEAEncode(encryptbuf, &encodeLen, orignalBuf, orignalBufLen, teaKey))
    {
        return false;
    }

    encodeStr.assign(encryptbuf, encodeLen);
    return true;
}

bool MessageXXTEAHelper::Decrypt(unsigned sessionID, const char* orignalBuf, size_t orignalBuflen, std::string& decodeStr)
{
    decodeStr.clear();
    const char* data = orignalBuf;
    const XXTEA_TYPE* teaKey = GetKey(sessionID);

    if (NULL == teaKey)
    {
        teaKey = defaultKey;
    }

    int decodeLen = MAX_DECODEBUF_LEN;
    if (!XXTEA::instance()->XXTEADecode(decryptbuf, &decodeLen, data, orignalBuflen, teaKey))
    {
        return false;
    }

    decodeStr.assign(decryptbuf, decodeLen);
    return true;
}

bool MessageXXTEAHelper::IsNeedUpdateKey(unsigned sessionID)
{
    if (0 == sessionID)
    {
        return false;
    }

    XXTEAKEYMAP::iterator keyMapIter = teaKeyMap.find(sessionID);
    if (keyMapIter != teaKeyMap.end())
    {
        XXTEAKEY& teaKeyDt = keyMapIter->second;
        unsigned long uNow = Time::GetCurSecond();
        if ((uNow - teaKeyDt.lastUpdTime) >= TEA_KEY_UPDATE_POINT)
        {
            return true;
        }
    }
    else
    {
        return true;
    }

    return false;
}

bool MessageXXTEAHelper::GenerateNewKey(XXTEA_TYPE* teaKey)
{
    if (NULL == teaKey)
    {
        return false;
    }

    teaKey[0] = FakeRandom::Instance().Random();
    teaKey[1] = FakeRandom::Instance().Random();
    teaKey[2] = FakeRandom::Instance().Random();
    teaKey[3] = FakeRandom::Instance().Random();

    return true;
}

bool MessageXXTEAHelper::UpdateKey(unsigned sessionID, const XXTEA_TYPE* teaNewKey)
{
    if (NULL == teaNewKey || 0 == sessionID)
    {
        return false;
    }

    XXTEAKEYMAP::iterator keyMapIter = teaKeyMap.find(sessionID);
    if (keyMapIter != teaKeyMap.end())
    {
        XXTEAKEY& teaKey = keyMapIter->second;
        teaKey.teaKey[0] = teaNewKey[0];
        teaKey.teaKey[1] = teaNewKey[1];
        teaKey.teaKey[2] = teaNewKey[2];
        teaKey.teaKey[3] = teaNewKey[3];

        UpdateLRU(teaKey.lastUpdTime, Time::GetCurSecond(), sessionID);

        teaKey.lastUpdTime = Time::GetCurSecond();
    }
    else
    {
        LRU();

        XXTEAKEY teaKey;
        teaKey.teaKey[0] = teaNewKey[0];
        teaKey.teaKey[1] = teaNewKey[1];
        teaKey.teaKey[2] = teaNewKey[2];
        teaKey.teaKey[3] = teaNewKey[3];
        teaKey.lastUpdTime = Time::GetCurSecond();

        UpdateLRU(teaKey.lastUpdTime, teaKey.lastUpdTime, sessionID);

        teaKeyMap[sessionID] = teaKey;
    }

    return teaKeyMap[sessionID].teaKey;
}

const XXTEA_TYPE* MessageXXTEAHelper::GetKey(unsigned sessionID)
{
    XXTEAKEYMAP::iterator keyMapIter = teaKeyMap.find(sessionID);
    if (keyMapIter != teaKeyMap.end())
    {
        return keyMapIter->second.teaKey;
    }

    return NULL;
}

void MessageXXTEAHelper::UpdateLRU(ull timeLast, ull timeNow, unsigned sessionId)
{
    LRUMAPSET::iterator lruIter = lruMapSet.find(timeLast);

    //first : 从旧时间链表删除借点
    if (lruIter != lruMapSet.end() && timeLast != timeNow)
    {
        std::set<unsigned>& sessionSet = lruIter->second;
        if (sessionSet.find(sessionId) != sessionSet.end())
        {
            sessionSet.erase(sessionId);
        }
        if (sessionSet.empty())
        {
            lruMapSet.erase(lruIter);
        }
    }

    //second : 插入新时间链表
    lruMapSet[timeNow].insert(sessionId);
}

void MessageXXTEAHelper::LRU()
{
    time_t   tNow = Time::GetCurSecond();
    if (((ULONG)tNow - lastLRUTime) < LRU_TIME_POINT)
    {
        return;
    }

    lastLRUTime = Time::GetCurSecond();

    LRUMAPSET::iterator lruIter = lruMapSet.begin();
    while (lruIter != lruMapSet.end())
    {
        ull timeLastUpd = lruIter->first;
        if (((ULONG)tNow - timeLastUpd) < SESSION_EXPIRE_GAP)
        {
            break;
        }

        std::set<unsigned>& lruSessionSet = lruIter->second;
        DoLRU(lruSessionSet);

        lruMapSet.erase(lruIter++);
    }
}

void MessageXXTEAHelper::DoLRU(std::set<unsigned>& lruSessionSet)
{
    std::set<unsigned>::iterator sit = lruSessionSet.begin();
    for ( ; sit != lruSessionSet.end(); ++sit)
    {
        DropSessionKey(*sit);
    }

    lruSessionSet.clear();
}

bool MessageXXTEAHelper::DropSessionKey(unsigned sessionID)
{
    XXTEAKEYMAP::iterator keyMapIter = teaKeyMap.find(sessionID);
    if (keyMapIter != teaKeyMap.end())
    {
        XXTEAKEY& teaKeyDt = keyMapIter->second;
        LOG_TRACE("Session LRU : session[%u] last update time[%s]", sessionID,
                Time::DateTimeStr((time_t *)&teaKeyDt.lastUpdTime));
        teaKeyMap.erase(sessionID);
    }

    return true;
}

