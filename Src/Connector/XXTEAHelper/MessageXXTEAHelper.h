/*
 * MessageXXTEAHelper.h
 *
 *  Created on: 2014年9月5日
 *      Author: silas
 */

#ifndef MESSAGEXXTEAHELPER_H_
#define MESSAGEXXTEAHELPER_H_

#include <Engine/Util/TMTSingleton.h>
#include <Engine/XXTEA/XXTEA.h>
#include <map>
#include <set>
#include <string>

class MessageXXTEAHelper : public TMTSingleton<MessageXXTEAHelper>
{
typedef unsigned long ull;
typedef struct tagXXTEAKEY
{
    XXTEA_TYPE          teaKey[4];
    ull                 lastUpdTime;
    tagXXTEAKEY()
    {
        teaKey[0] = teaKey[1] = teaKey[2] = teaKey[3] = 0;
        lastUpdTime = 0;
    }
} XXTEAKEY;

public:
    MessageXXTEAHelper();
    virtual             ~MessageXXTEAHelper();

public:
    /*
     * @brief   :       加密二进制数据
     *
     * @param   sessionID   :   回话ID
     * @param   orignalBuf  :   原始二进制数据buf
     * @param   orignalBufLen   :   二进制数据长度
     * @param   encodeStr   :   解密后数据
     *
     * @return  :   加密结果[true : 成功 false : 失败]
     *
     * */
    bool                Encrypt(unsigned sessionID, const char* orignalBuf, size_t orignalBufLen, std::string& encodeStr);

    /*
     * @brief   :       解密二进制数据
     *
     * @param   sessionID   :   回话ID
     * @param   orignalBuf  :   待解密数据buf
     * @param   orignalBufLen   :   待解密数据长度
     * @param   decodeStr   :   解密后数据
     *
     * @return  :   揭秘结果[]
     *
     * */
    bool                Decrypt(unsigned sessionID, const char* orignalBuf, size_t orignalBuflen, std::string& decodeStr);

public:
    /*
     * @brief   :       是否需要更新加密key
     *
     * @param sessionID :   回话ID
     *
     * @return  :       true : 需要 false : 无需
     *
     * */
    bool                IsNeedUpdateKey(unsigned sessionID);

    /*
     * @brief   :       生成新加解密key
     *
     * @param teaKey    :   新key存储buf
     *
     * @return  :       生成结果[true : 成功 false : 失败]
     *
     * */
    bool                GenerateNewKey(XXTEA_TYPE* teaKey);

    /*
     * @brief   :       更新制定session加解密key
     *
     * @param   sessionID   :   回话id
     * @param   teaNewKey   :   新加解密key
     *
     * @return  :       更新结果[true : 成功 false : 失败]
     *
     * */
    bool                UpdateKey(unsigned sessionID, const XXTEA_TYPE* teaNewKey);

private:
    /*
     * @brief   :       获取对称加密KEY
     *
     * @param sessionID :   回话ID
     *
     * @return  :       获取结果[!NULL : 加解密KEY NULL : 获取KEY失败]
     *
     * */
    const XXTEA_TYPE*   GetKey(unsigned sessionID);

    /*
     * @brief   :       更新LRU链
     *
     * @param timeLast  :   上次更新时间
     * @param timeNow   :   当前更新时间
     * @param sessionID :   sessionid
     *
     * */
    void                UpdateLRU(ull timeLast, ull timeNow, unsigned sessionId);

    /*
     * @brief   :       淘汰无效session
     *
     * */
    void                LRU();

    /*
     * @brief   :       执行淘汰
     *
     * @param lruSessionSet :   待淘汰session列表
     *
     * */
    void                DoLRU(std::set<unsigned>& lruSessionSet);

    /*
     * @brief   :       删除制定回话加解密key
     *
     * @param sessionID :   回话id
     *
     * @return  :       删除结果[true : 成功 false : 失败]
     *
     * */
    bool                DropSessionKey(unsigned sessionID);

private:
    typedef std::map<unsigned, XXTEAKEY> XXTEAKEYMAP;       // session->key
    typedef std::map<ull, std::set<unsigned> > LRUMAPSET;   // time->session list

private:
    XXTEAKEYMAP         teaKeyMap;
    LRUMAPSET           lruMapSet;
    XXTEA_TYPE          defaultKey[4];
    ull			        lastLRUTime;
    char*               encryptbuf;
    char*               decryptbuf;
};

#endif /* MESSAGEXXTEAHELPER_H_ */
