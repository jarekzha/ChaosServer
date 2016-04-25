/*
 * DefaultGMConnectorMessageHelper.cpp
 *
 *  Created on: 2014年8月29日
 *      Author: silas
 */

#include <Engine/Log/LogMacro.h>
#include <Engine/Util/Util.h>
#include <Common/Util/HTTPUtil.h>
#include <Framework/GameServer.h>
#include <Message/Message/GMMessage.h>
#include <Player/Player.h>
#include <GameMaster/GameMasterModule.h>
#include <Src/GameServer/Message/Helper/DefaultGMConnectorMessageHandler.h>

DefaultGMConnectorMessageHelper::DefaultGMConnectorMessageHelper()
{
    // NULL
}

DefaultGMConnectorMessageHelper::~DefaultGMConnectorMessageHelper()
{
    // NULL
}

void DefaultGMConnectorMessageHelper::OnGetCreateAsyncData(AsyncDataList& asyncDataList)
{
    // NULL
}

void DefaultGMConnectorMessageHelper::HandleMessage()
{
    GMMessage* gmmessage = dynamic_cast<GMMessage *>(message);
    ASSERT_RETURN_VOID(NULL != gmmessage);

    GameMasterModule& gmModule = theModule(GameMasterModule);

    const std::string& httpRequest = gmmessage->GetHttpRequestContent();
    if (httpRequest.empty())
    {
        return;
    }

    std::string decodeHttpUrl;
    if (!HTTPUtil::Instance().DecodeHTTPUrlString(httpRequest.c_str(), httpRequest.length(), decodeHttpUrl))
    {
        LOG_ERR("[GMHTTP] decode url[%s] fail", httpRequest.c_str());
        return;
    }

    std::vector<std::string> httpCommandVec;
    if (!HTTPUtil::Instance().Split(decodeHttpUrl, "\r\n", httpCommandVec))
    {
        LOG_ERR("[GMHTTP] split url[%s] fail", httpRequest.c_str());
        return;
    }

    std::vector<std::string>::iterator httpCommandVecIter = httpCommandVec.begin();
    for ( ; httpCommandVecIter != httpCommandVec.end(); ++httpCommandVecIter)
    {
        std::string& command = *httpCommandVecIter;
        std::vector<std::string> httpCommandParamVec;
        if (!HTTPUtil::Instance().Split(command, " ", httpCommandParamVec))
        {
            LOG_ERR("[GMHTTP] split command[%s] fail", command.c_str());
            continue;
        }

        //command format : command name param[1~n]
        if (httpCommandParamVec.empty() || httpCommandParamVec.size() <= 2)
        {
            LOG_ERR("[GMHTTP] command[%s] param num[%u] invalid", command.c_str(), (unsigned)httpCommandParamVec.size());
            continue;
        }

        gmModule.HandleGMRequest(httpCommandParamVec);
    }
}

void DefaultGMConnectorMessageHelper::HandleDataTimeOut()
{

}
