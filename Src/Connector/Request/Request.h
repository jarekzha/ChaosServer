/*
 * Request.h
 *
 *  Created on: 2014年7月24日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Time/Timer.h>
#include <Engine/Language/Visualizable.h>
#include <fcgiapp.h>
#include <Protocol/NetMsg.pb.h>

#ifdef _DEBUG
#define WAIT_RSP_TIMEOUT            (60*TIMER_SECOND)   // 等待10s
#else
#define WAIT_RSP_TIMEOUT            (10*TIMER_SECOND)   // 等待应答10s超时
#endif

class Request : public Visualizable
{
public:
    Request();
    virtual             ~Request();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

public:
    bool                Initialize();

    void                Reset();

    bool                WaitAccept();

    void                SendToGameServer();

    void                DoResponse(const MsgSet& responseMsg);

    void                SetRequestID(int requestID);
    int                 GetRequestID();

private:
    void                Finish();

    bool                ReadRequest();

    void                OnWaitResponseTimeout();

    bool                EncryptTransferMessage(CSTransferMsg& transferMsg, const MsgSet& responseMsg);
    bool                DecryptTransferMessage(const char* serializeMsg, int serializeLen);

private:
    int                 requestID;
    FCGX_Request        request;
    bool                recvRequest;        // 已接收到request
    MsgSet              reqMsgSet;          // 接收到的request消息包
    Timer               waitRspTimeoutTimer;
};

