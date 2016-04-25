/*
 * HTTPUtil.h
 *
 *  Created on: 2014年8月31日
 *      Author: silas
 */

#pragma once

#include <Util/TMTSingleton.h>
#include <string>
#include <vector>

class HTTPUtil : public TMTSingleton<HTTPUtil>
{
public:
    HTTPUtil();
    virtual ~HTTPUtil();

public:
    /*
     * @brief : 解码http请求串
     *
     * @param encodeUrl : 带解码原始请求串
     * @param encodeUrlLen : 待解码原始请求串长度
     * @param decodeUrl : 解码后请求串
     *
     * @return : 解码结果[true : 成功 false : 失败]
     *
     * */
    bool DecodeHTTPUrlString(const char* encodeUrl, int encodeUrlLen, std::string& decodeUrl);

    /*
     * @brief : 才分字符串
     *
     * @param srcStr : 待才分原始字符串
     * @param separate : 才分时制定的分隔符
     * @param strVec : 才分后的字符串列表
     *
     * @param return : 才分结果[true : 成功 false : 失败]
     *
     * */
    bool Split(const std::string& srcStr, const std::string& separate, std::vector<std::string>& strVec);

private:
    /*
     * @brief : 字符转义
     *
     * @param hex : 待转义十六进制字符
     *
     * @return : 转义后字符
     *
     * */
    char TranslateHex(char hex);
};

