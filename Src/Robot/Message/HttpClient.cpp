/*
 * HttpClient.cpp
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief:
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Engine/Language/String.h>
#include <Engine/XXTEA/XXTEA.h>
#include <Extern/Zlib/zlib.h>
#include <google/protobuf/message_lite.h>
#include <Message/HttpClient.h>
#include <Player/RobotPlayer.h>
#include <stddef.h>
#include <cassert>
#include <cstdio>
#include <string>

using namespace std;


HttpClient::HttpClient()
{
    debug = false;
    serverPort = 0;
    httpHeaders = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
}

HttpClient::~HttpClient()
{
    if (httpHeaders != NULL)
    {
        curl_slist_free_all(httpHeaders);
    }
}

void HttpClient::Initialize(const string& serverHost, WORD serverPort, bool debug)
{
    this->debug = debug;
    this->serverHost = serverHost;
    this->serverPort = serverPort;

    httpHeaders = curl_slist_append(httpHeaders, "Content-Type:application/octet-stream");
}

bool HttpClient::Post(const RobotRequest& request, RobotResponse* response)
{
    std::string encryptStr;
    if (!EncryptMessage(request, encryptStr))
    {
        return false;
    }

    CSTransferMsg transferMsg;
    transferMsg.set_session_id(request.GetMsgSet().head().session_id());
    transferMsg.set_compress(0);

    char compressBuf[2048];
    uLongf compressLen = sizeof(compressBuf);
    int compressRet = compress((Bytef *)compressBuf, &compressLen, (const Bytef *)encryptStr.c_str(), (uLong)encryptStr.size());
    if (Z_OK == compressRet)
    {
        transferMsg.set_msg(std::string(compressBuf, compressLen));
        transferMsg.set_compress(1);
//        LOG_TRACE("Request<%s> compress succuss, orignal-size[%u] compress-len[%d]",
//                request.BriefInfo().c_str(), (UINT)encryptStr.size(), (int)compressLen);
    }
    else
    {
        transferMsg.set_msg(encryptStr);
        LOG_ERR("Request<%s> compress fail, ret[%d]", request.BriefInfo().c_str(), compressRet);
    }

    string responseMsgStr;
    std::string requestMsgStr;
    if (!transferMsg.SerializeToString(&requestMsgStr))
    {
        LOG_ERR("Request<%s> serialize to string fail, encyptsize[%d]",
                request.BriefInfo().c_str(), (uint)encryptStr.size());
        return false;
    }

    CURL* curl = curl_easy_init();
    if (NULL == curl)
    {
        LOG_ERR("Curl easy init fail");
        assert(false);
        return false;
    }

    if (debug)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, HttpClient::OnDebug);
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, httpHeaders);
    curl_easy_setopt(curl, CURLOPT_URL, serverHost.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestMsgStr.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestMsgStr.size());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&responseMsgStr);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    //curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    //curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK)
    {
        LOG_ERR("Post perform fail<res:%d>", res);
        assert(false);
        return false;
    }

    std::string decryptStr;
    if (!DecryptMessage(request, responseMsgStr, decryptStr))
    {
        LOG_ERR("Decrypt response message<%s> to request<%s> fail",
                BinaryToReadableString(responseMsgStr.c_str(), responseMsgStr.size()).c_str(),
                request.BriefInfo().c_str());
        return false;
    }

    RobotResponse tempResponse;
    if (NULL == response)
    {
        response = &tempResponse;
    }

    if (!response->ParseMsg(decryptStr))
    {
        LOG_ERR("Response msg to request<%s> parse fail", request.BriefInfo().c_str());
        return false;
    }

    request.GetRobotPlayer().SetSequence(response->GetSequenceID());

    MsgSet& msgSet = response->GetMsgSet();
    PreHandleMessage(request, msgSet);

    return true;
}


int HttpClient::OnDebug(CURL* curl, curl_infotype type, char* data, size_t size, void *)
{
    switch (type)
    {
        case CURLINFO_TEXT:
        {
            LOG_TRACE("Http <text:%s>", data);
            break;
        }
        case CURLINFO_HEADER_IN:
        {
            LOG_TRACE("Http <headerIn:%s>", data);
            break;
        }
        case CURLINFO_HEADER_OUT:
        {
            LOG_TRACE("Http <headerOut:%s>", data);
            break;
        }
        case CURLINFO_DATA_IN:
        {
            LOG_TRACE("Http <dataIn:%s>", data);
            break;
        }
        case CURLINFO_DATA_OUT:
        {
            LOG_TRACE("Http <dataOut:%s>", data);
            break;
        }
        default: break;
    }

    return 0;
}

size_t HttpClient::OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    string* str = (string*)lpVoid;
    ASSERT_RETURN(str != NULL && buffer != NULL, 0);

    str->append((char*)buffer, size * nmemb);
    return nmemb;
}

bool HttpClient::EncryptMessage(const RobotRequest& request, std::string& encryptMsg)
{
    string requestMsgStr;
    if (!request.GetMsgSet().SerializeToString(&requestMsgStr))
    {
        LOG_ERR("Request<%s> serialize to string fail", request.BriefInfo().c_str());
        return false;
    }

    int encodeLen = 1024;
    char msgBuf[1024] = {0};
    const XXTEA_TYPE* encryptKey = request.GetRobotPlayer().GetDefaultKey();
    if (request.GetRobotPlayer().IsKeyInit())
    {
        encryptKey = request.GetRobotPlayer().GetTeaKey();
    }

    if (!XXTEA::instance()->XXTEAEncode(msgBuf, &encodeLen, requestMsgStr.c_str(), requestMsgStr.size(), encryptKey))
    {
        LOG_ERR("Request<%s> encrypt fail", request.BriefInfo().c_str());
        return false;
    }

    encryptMsg.assign(msgBuf, encodeLen);
    return true;
}

bool HttpClient::DecryptMessage(const RobotRequest& request, const std::string& responseMsgStr, std::string& decryptStr)
{
    CSTransferMsg transferMsg;
    if (!transferMsg.ParseFromString(responseMsgStr))
    {
        return false;
    }

    std::string uncompressStr(transferMsg.msg());
    if (0 != transferMsg.compress())
    {
        char uncompressBuf[1024];
        uLongf uncompressLen = sizeof(uncompressBuf);
        int uncompressRet = uncompress((Bytef *)uncompressBuf, &uncompressLen, (const Bytef *)uncompressStr.c_str(), (uLong)uncompressStr.size());
        if (Z_OK != uncompressRet)
        {
            LOG_ERR("Request<%s> uncompress fail, ret[%d]", request.BriefInfo().c_str(), uncompressRet);
            return false;
        }
        uncompressStr.clear();
        uncompressStr.assign(uncompressBuf, uncompressLen);
    }

    int decodeLen = 1024;
    char msgBuf[1024] = {0};
    if (!XXTEA::instance()->XXTEADecode(msgBuf, &decodeLen, uncompressStr.c_str(), uncompressStr.size(), request.GetRobotPlayer().GetTeaKey()))
    {
        if (!XXTEA::instance()->XXTEADecode(msgBuf, &decodeLen, uncompressStr.c_str(), uncompressStr.size(), request.GetRobotPlayer().GetDefaultKey()))
        {
            return false;
        }
    }

    decryptStr.assign(msgBuf, decodeLen);
    return true;
}

void HttpClient::PreHandleMessage(const RobotRequest& request, const MsgSet& msgSet)
{
    for (int i = 0; i < msgSet.msg_size(); ++i)
    {
        const NetMsg& msg = msgSet.msg(i);
        if (msg.id() == CS_MSG_CHG_DCRYPTKEY_NTF)
        {
            const CSChgDecryptKeyNtf& chgKeyNtf = msg.body().cs_chg_decrypt_key_ntf();
            XXTEA_TYPE newKey[4] = {0};
            for (int j = 0; j < chgKeyNtf.key_size(); ++j)
            {
                newKey[j] = chgKeyNtf.key(j);
            }
            request.GetRobotPlayer().SetTeaKey(newKey);
            LOG_TRACE("player<%s> decryptKey<%2x %2x %2x %2x> msg sequence<%u> changed",
                    request.GetRobotPlayer().BriefInfo().c_str(),
                    newKey[0], newKey[1], newKey[2], newKey[3],
                    msgSet.head().sequence());
        }
    }
}
