/*
 * HttpClient.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <Extern/Curl/curl.h>
#include <Message/RobotRequest.h>
#include <Message/RobotResponse.h>
#include <Engine/Util/TMTSingleton.h>
#include <string>


class HttpClient : public TMTSingleton<HttpClient>
{
public:
    HttpClient();
    virtual             ~HttpClient();

public:
    void                Initialize(const std::string& serverHost, WORD serverPort, bool debug);

    bool                Post(const RobotRequest& request, RobotResponse* response);

private:
    static int          OnDebug(CURL* curl, curl_infotype type, char* data, size_t size, void *);

    static size_t       OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid);

private:
    bool                EncryptMessage(const RobotRequest& request, std::string& encryptMsg);
    bool                DecryptMessage(const RobotRequest& request, const std::string& responseMsgStr, std::string& decryptStr);
    void                PreHandleMessage(const RobotRequest& request, const MsgSet& msgSet);

private:
    bool                debug;
    std::string         serverHost;
    WORD                serverPort;

    struct curl_slist*  httpHeaders;
};


