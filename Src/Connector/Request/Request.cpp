/*
 * Request.cpp
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief:
 */

#include <Connector/Framework/Connector.h>
#include <Connector/Request/DelayReleaseRequest.h>
#include <Connector/Request/Request.h>
#include <Connector/Util/ConnectorConfig.h>
#include <Connector/XXTEAHelper/MessageXXTEAHelper.h>
#include <Engine/Delay/DelayFunction.h>
#include <Engine/IPC/Define.h>
#include <Engine/IPC/IPCHandle.h>
#include <Engine/Log/LogMacro.h>
#include <Engine/Object/New1.h>
#include <Engine/Time/TimerCallback.h>
#include <Engine/Util/Util.h>
#include <Extern/Zlib/zconf.h>
#include <Extern/Zlib/zlib.h>
#include <google/protobuf/repeated_field.h>
#include <Protocol/Macro.pb.h>
#include <Protocol/MsgID.pb.h>
#include <cstring>
#include <ctime>
#include <sstream>
#include <string>
#include <errno.h>

using namespace std;

#define COMPRESS_CRITICAL_SECTION   (1024)
#define __DEBUG__


Request::Request()
{
    requestID = 0;
    recvRequest = false;
}

Request::~Request()
{
    Reset();
    FCGX_Free(&request, 1);
}

std::string Request::BriefInfo() const
{
    std::stringstream briefInfo;
    briefInfo<<"requestID:"<<requestID;

    return briefInfo.str();
}

bool Request::Initialize()
{
    if (FCGX_InitRequest(&request, 0, 0) != 0)
    {
        LOG_ERR("Request init fail:%s", strerror(errno));
        return false;
    }

    return true;
}

void Request::Reset()
{
    requestID = 0;
    reqMsgSet.Clear();
    Finish();
}

bool Request::WaitAccept()
{
    int result = FCGX_Accept_r(&request);
    if (result < 0)
    {
        LOG_ERR("Request wait fail<result:%d>", result);
        return false;
    }

    recvRequest = true;
    waitRspTimeoutTimer.Start(TimerCallback(this, &Request::OnWaitResponseTimeout), WAIT_RSP_TIMEOUT);

    if (!ReadRequest())
    {
        Finish();
        return false;
    }

    LOG_RUN("Message <sessionID:%d, sequence:%d, id:%s> recv request<id:%d>",
            reqMsgSet.head().session_id(),
            reqMsgSet.head().sequence(),
            MsgID_Name(reqMsgSet.msg().Get(0).id()).c_str(),
            requestID);

    return true;
}

void Request::SendToGameServer()
{
    const ConnectorConfigItem& config = theConnector.GetConfigItem();
    IPCHandle& ipcHandle = theConnector.GetIPCHandle();

    ipcHandle.SendMessage(config.GameServerProcID, reqMsgSet);
}


void Request::DoResponse(const MsgSet& responseMsg)
{
    CSTransferMsg transferMsg;
    if (!EncryptTransferMessage(transferMsg, responseMsg))
    {
        return;
    }

    char msgSetData[MSG_CACHE_SIZE];
    if (!transferMsg.SerializeToArray(msgSetData, sizeof(msgSetData)))
    {
        LOG_TRACE("TransferMessage <sessionID:%u sequence:%u> serialize fail",
                responseMsg.head().session_id(), responseMsg.head().sequence());
        return;
    }

    int hopeWriteLen = transferMsg.ByteSize();
    FCGX_PutS("Content-Type:application/octet-stream\r\n\r\n", request.out);
    int writeLen = FCGX_PutStr(msgSetData, hopeWriteLen, request.out);
    if (writeLen < hopeWriteLen)
    {
        LOG_ERR("Message <sessionID:%d sequence:%d id:%d> write fail",
                responseMsg.head().session_id(), responseMsg.head().sequence(), responseMsg.msg().Get(0).id());
        LOG_TRACE("Message write fail <%s>", reqMsgSet.DebugString().c_str());
        return;
    }

    Finish();

    LOG_RUN("Message <sessionID:%d, sequence:%d, id:%s> response to"
            " request <sessionID:%d, sequence:%d, id:%s, requestID:%d writelen:%d hopeLen:%d>",
            responseMsg.head().session_id(),
            responseMsg.head().sequence(),
            MsgID_Name(responseMsg.msg().Get(0).id()).c_str(),
            reqMsgSet.head().session_id(),
            reqMsgSet.head().sequence(),
            MsgID_Name(reqMsgSet.msg().Get(0).id()).c_str(),
            requestID, writeLen, hopeWriteLen);
}

void Request::SetRequestID(int requestID)
{
    this->requestID = requestID;
}

int Request::GetRequestID()
{
    return requestID;
}

void Request::Finish()
{
    if (recvRequest)
    {
        FCGX_Finish_r(&request);
        waitRspTimeoutTimer.Stop();
    }

    recvRequest = false;
}

bool Request::ReadRequest()
{
    char msgSetData[MSG_CACHE_SIZE];
    int readLen = 0;

    // 读取全包
    readLen = FCGX_GetStr(msgSetData, MSG_CACHE_SIZE, request.in);
    if (MSG_CACHE_SIZE == readLen)
    {
        LOG_ERR("Request<%s> read pkg full, maybe cache<size:%d> is small",
                BriefInfo().c_str(), MSG_CACHE_SIZE);
        return false;
    }

    if (!DecryptTransferMessage(msgSetData, readLen))
    {
        return false;
    }

    // 协议包中，附加requestID
    reqMsgSet.mutable_extra()->mutable_cs_msg()->set_request_id(requestID);

    return true;
}

void Request::OnWaitResponseTimeout()
{
    LOG_TRACE("Request<id:%d> wait response timeout", requestID);

    MsgSet rspMsgSet;
    rspMsgSet.mutable_head()->set_session_id(reqMsgSet.head().session_id());
    rspMsgSet.mutable_head()->set_sequence(reqMsgSet.head().sequence() + 1);
    NetMsg* rspMsg = rspMsgSet.mutable_msg()->Add();
    rspMsg->set_id(CS_MSG_ERROR_NTF);
    rspMsg->mutable_body()->mutable_cs_error_ntf()->set_error_id(MSG_ERROR_KICKOUT);

    DoResponse(rspMsgSet);

    DelayFunctionInvoker::Invoke(New<DelayReleaseRequest>(this));
}

bool Request::EncryptTransferMessage(CSTransferMsg& transferMsg, const MsgSet& responseMsg)
{
    UINT sessionID = responseMsg.head().session_id();
    MsgSet resMsgSet;

    bool teaKeyChged = false;
    XXTEA_TYPE teaNewKey[4] = {0};
#if 0
    if (MessageXXTEAHelper::Instance().IsNeedUpdateKey(sessionID))
    {
        Msg& msg = *resMsgSet.mutable_msg()->Add();
        msg.set_id(CS_MSG_CHG_DCRYPTKEY_NTF);

        CSChgDecryptKeyNtf& chgDecryptKeyNtf = *msg.mutable_body()->mutable_cs_chg_decrypt_key_ntf();
        MessageXXTEAHelper::Instance().GenerateNewKey(teaNewKey);
        for (UINT i = 0; i < sizeof(teaNewKey)/sizeof(XXTEA_TYPE); ++i)
        {
            chgDecryptKeyNtf.add_key(teaNewKey[i]);
        }
        teaKeyChged = true;

        LOG_TRACE("Session<%u> change encrypt key", sessionID);
    }
#endif

    resMsgSet.MergeFrom(responseMsg);

    char msgSetData[MSG_CACHE_SIZE];
    if (!resMsgSet.SerializeToArray(msgSetData, sizeof(msgSetData)))
    {
        LOG_ERR("Message <sessionID:%d sequence:%d id:%d> serialize fail",
                responseMsg.head().session_id(), responseMsg.head().sequence(), responseMsg.msg().Get(0).id());
        LOG_TRACE("Message serialize fail <%s>", reqMsgSet.DebugString().c_str());
        return false;
    }

    string encryptStr;
    if (!MessageXXTEAHelper::Instance().Encrypt(sessionID, msgSetData, resMsgSet.ByteSize(), encryptStr))
    {
        LOG_ERR("Message <sessionID:%u sequence:%u> encrypt fail",
                responseMsg.head().session_id(), responseMsg.head().sequence());
        return false;
    }

    if (teaKeyChged)
    {
        MessageXXTEAHelper::Instance().UpdateKey(sessionID, teaNewKey);
    }

    transferMsg.set_session_id(sessionID);
    transferMsg.set_compress(0);
    transferMsg.set_msg(encryptStr);

#if 0
    if (encryptStr.size() >= COMPRESS_CRITICAL_SECTION)
    {
        char compressBuf[MSG_CACHE_SIZE];
        uLongf compressLen = MSG_CACHE_SIZE;
        int compressRet = compress((Bytef *)compressBuf, &compressLen, (const Bytef *)encryptStr.c_str(), (uLong)encryptStr.size());
        if (Z_OK == compressRet)
        {
            transferMsg.set_compress(1);
            transferMsg.set_msg(std::string(compressBuf, (int)compressLen));
        }
    }
#endif

    return true;
}

bool Request::DecryptTransferMessage(const char* serializeMsg, int serializeLen)
{
    CSTransferMsg transferMsg;
    if (!transferMsg.ParseFromArray(serializeMsg, serializeLen))
    {
        LOG_ERR("Request<%s> parse transfer msg fail", BriefInfo().c_str());
        return false;
    }

    UINT sessionID = transferMsg.session_id();
    const std::string requestStr = transferMsg.msg();
    std::string uncompressStr(requestStr);
    if (0 != transferMsg.compress())
    {
        char uncompressBuf[MSG_CACHE_SIZE*2];
        uLongf uncompressLen = sizeof(uncompressBuf);
        int uncompressRet = uncompress((Bytef *)uncompressBuf, &uncompressLen, (Bytef *)requestStr.c_str(), (uLong)requestStr.size());
        if (Z_OK != uncompressRet)
        {
            LOG_ERR("Request<%s> uncompress fail, ret=%d", BriefInfo().c_str(), uncompressRet);
            return false;
        }
        uncompressStr.clear();
        uncompressStr.assign(uncompressBuf, (int)uncompressLen);
    }

    std::string decryptStr;
    if (!MessageXXTEAHelper::Instance().Decrypt(sessionID, uncompressStr.c_str(), uncompressStr.size(), decryptStr))
    {
        LOG_ERR("Request<%s> decrypt fail", BriefInfo().c_str());
        return false;
    }


    // 解析全包
    if (!reqMsgSet.ParseFromArray(decryptStr.c_str(), decryptStr.length()))
    {
        LOG_ERR("Request<%s> parse msg set fail, sessionid[%u]", BriefInfo().c_str(), sessionID);
        return false;
    }

    if (sessionID != reqMsgSet.head().session_id())
    {
        LOG_ERR("Request<%s> session id not matched", BriefInfo().c_str());
        return false;
    }

    return true;
}
