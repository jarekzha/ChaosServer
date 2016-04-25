/*
 * IPCZMQTunnel.cpp
 *
 *  Created on: 2015年11月11日
 *  Author: seekerwang
 *  Brief: 
 */

#include <Util/Util.h>
#include <Log/LogMacro.h>
#include <Object/New.h>
#include <sstream>
#include <zmq.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <IPC/IPCConfig.h>
#include <IPC/IPCZMQTunnel.h>
#include <Util/ProtoBuf.h>

#define ZMQ_SEND_RECV_TIMEOUT   (100)   //100ms

using namespace std;

IPCZMQTunnel::IPCZMQTunnel()
    : config(NULL)
    , index(0)
    , selfProcID(0)
    , remoteProcID(0)
    , zmqContext(NULL)
    , zmqSocket(NULL)
{
    //Create ZeroMQ Context
    zmqContext = zmq_ctx_new ();
    assert(NULL != zmqContext);

    //Create local ZeroMQ IPC Pair
    if (NULL != zmqContext)
    {
        zmqSocket = zmq_socket (zmqContext, ZMQ_PAIR);
        assert(NULL != zmqSocket);
    }
}

IPCZMQTunnel::~IPCZMQTunnel()
{
    //Close
    if (NULL != zmqSocket)
    {
        zmq_close(zmqSocket);
    }

    //Destroy
    if (NULL != zmqContext)
    {
        zmq_ctx_term(zmqContext);
    }
}

std::string IPCZMQTunnel::BriefInfo() const
{
    stringstream briefInfo;
    briefInfo << "remoteProcID:" << remoteProcID;
    return briefInfo.str();
}

bool IPCZMQTunnel::Initialize(int index, int selfProcID, const std::string& namePrefix, const IPCChannelConfigItem& config)
{
    ASSERT_RETURN(index >= 0 && selfProcID >= 0, false);
    if (NULL == zmqSocket || NULL == zmqContext)
    {
        return false;
    }

    //Set local ZeroMQ Rcv timeout option
    int timeout = ZMQ_SEND_RECV_TIMEOUT;
    int ret = zmq_setsockopt (zmqSocket, ZMQ_RCVTIMEO, &timeout, sizeof (int));
    ASSERT_RETURN(0 == ret, false);

    //Set remote ZeroMQ Send timeout option
    ret = zmq_setsockopt (zmqSocket, ZMQ_SNDTIMEO, &timeout, sizeof (int));
    ASSERT_RETURN(0 == ret, false);

    this->config = &config;
    this->index = index;
    this->selfProcID = selfProcID;

    sockaddr_in addr;
    addr.sin_addr.s_addr = config.Proc2;    //dest channel address
    const char* s = inet_ntoa(addr.sin_addr);
    std::string ipcEntity("ipc:///tmp/"+ namePrefix + "_" + s);
    if (config.Proc1 == selfProcID)         //Connect
    {
        remoteProcID = config.Proc2;
        ret = zmq_connect (zmqSocket, ipcEntity.c_str());
        LOG_RUN("IPCZMQTunnel<%s> initialize done, connect ipc<%s> ret = %d",
                BriefInfo().c_str(), ipcEntity.c_str(), ret);
    }
    else if (config.Proc2 == selfProcID)    //Bind
    {
        remoteProcID = config.Proc1;
        ret = zmq_bind (zmqSocket, ipcEntity.c_str());
        LOG_RUN("IPCZMQTunnel<%s> initialize done, bind ipc<%s> ret = %d",
                BriefInfo().c_str(), ipcEntity.c_str(), ret);
    }
    else
    {
        LOG_ERR("IPCZMQTunnel<%s> init fail, invalid config", BriefInfo().c_str());
        return false;
    }

    return (0 == ret) ? true : false;
}

bool IPCZMQTunnel::SendMsg(const void* data, size_t dataLen)
{
    ASSERT_RETURN(data != NULL && dataLen > 0, false);
    if (NULL == zmqSocket)
    {
        LOG_ERR("ZeroMQ Socket Invalid");
        return false;
    }

    int writeLen = zmq_send (zmqSocket, data, dataLen, 0);
    if (writeLen == (int)dataLen)
    {
        return true;
    }

   LOG_ERR("IPCZMQTunnel<%s> send msg<size:%d> fail<%s>",
            BriefInfo().c_str(), (int)dataLen, strerror(zmq_errno()));

   return false;
}

bool IPCZMQTunnel::TryRecvMsg(void* buffer, size_t& recvLen, size_t bufferLen)
{
    ASSERT_RETURN(buffer != NULL && bufferLen > 0, false);
    if (NULL == zmqSocket)
    {
        LOG_ERR("ZeroMQ Socket Invalid");
        return false;
    }

    int rcvLen = zmq_recv(zmqSocket, buffer, bufferLen, ZMQ_DONTWAIT);
    recvLen = rcvLen;
    if (rcvLen > 0)
    {
        return true;
    }

    if (rcvLen < 0 && EAGAIN == zmq_errno())
    {
        recvLen = 0;
        return true;
    }

    LOG_ERR("IPCZMQTunnel<%s> try recv msg fail<%s>",
            BriefInfo().c_str(), strerror(zmq_errno()));
    return false;
}

int IPCZMQTunnel::GetRemoteProcID() const
{
    return remoteProcID;
}
