/*
 * HTTPRequest.h
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_
#include <fcgiapp.h>
#include <string>

class HTTPRequest
{
public:
    HTTPRequest();
    virtual ~HTTPRequest();

public:
    /*
     * @brief : 初始化HTTPRequest
     *
     * @return : 初始化结果[true : 成功 false : 失败]
     *
     * */
    bool Initialize();

    /*
     * @brief : 处理HTTP请求
     *
     * @return : 处理结果[true : 有请求 false : 无请求-idle]
     *
     * */
    bool DispatchHTTPRequest();

private:
    /*
     * @brief : HTTP请求处理完成，资源回收释放
     *
     **/
    void Finish();

    /*
     * @brief : 读取HTTP请求报文
     *
     * @return : 读取结果[true : 陈功 false : 失败]
     *
     * */
    bool ReadRequest();

    /*
     * @brief : 处理http请求
     *
     * @return : 处理结果[true : 成功 false : 失败]
     *
     **/
    bool HandleHTTPRequest();

    /*
     * @brief : 将收到的http请求转发至GameSvr
     *
     * @param commandStr : http请求串
     *
     * @return : 转发结果[true : 成功 false : 失败]
     *
     * */
    bool ForwardToGameSvr(const std::string& commandStr);

    /*
     * @brief : 默认响应
     *
     * @return : 响应结果[true : 成功 false : 失败]
     *
     **/
    bool DefaultResponse();

private:
    char*               httpRequstBuf;
    int                 httpRequestBuflen;
    FCGX_Request        fastcgiRequestHandle;
};

#endif /* HTTPREQUEST_H_ */
