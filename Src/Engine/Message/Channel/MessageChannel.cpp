/*
 * MessageChannel.cpp
 *
 *  Created on: 2015年11月10日
 *  Author: jarek
 *  Brief:
 */

#include <Message/Channel/MessageChannel.h>


MessageChannel::MessageChannel()
{
    localProcID = 0;
    remoteProcID = 0;
}

int MessageChannel::GetRemoteProcID() const
{
    return remoteProcID;
}

int MessageChannel::GetLocalProcID() const
{
    return localProcID;
}

void MessageChannel::InitProcID(int localProcID, int remoteProcID)
{
    this->localProcID = localProcID;
    this->remoteProcID = remoteProcID;
}
