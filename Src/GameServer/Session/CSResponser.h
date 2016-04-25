
/*
 * Responser.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Engine/Util/Util.h>
#include <list>
#include <Protocol/NetMsg.pb.h>

class CSRequest;
class PlayerSession;

/*
 * 请求应答者，针对单次request，可能会有多个应答，此应答者将所有应答统一管理
 */
class CSResponser
{
public:
    CSResponser();
    virtual             ~CSResponser();

public:
    void                Init();

    // 开始应答
    bool                StartResponse(CSRequest* request);

    // 添加子应答
    void                AddSubResponse(const NetMsg& msg);

    // 获取当前请求
    CSRequest*          GetCurRequest();

    // 结束应答，将所有的应答包一起下发
    void                TryFinishResponse(PlayerSession* session);

    // 中止应答，删除请求信息，和缓存的应答包
    void                AbortResponse();

    // 检测请求的序列号是否合法
    bool                CheckRequestSequence(CSRequest* request);

    // 获取上次请求时间
    ULONG               GetLastRequestSec();

    // 获取此刻上行包合法的sequence
    int                 GetValidRequestSequence();

    // 有请求等待应答
    bool                HaveRequest();

private:
    typedef std::list<NetMsg> ResponseMsgList;

    // 请求相关信息
    int                     validRequestSequence;
    ULONG                   lastRequestSec;     // 上次的请求时间(s)
    CSRequest*              curRequest;         // 当前正在应答的请求
    ResponseMsgList         responseMsgList;    // 应答的消息列表
};

