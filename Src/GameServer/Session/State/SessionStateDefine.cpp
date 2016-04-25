
/*
 * SessionStateDefine.cpp
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */



#include <Session/State/SessionOfflineState.h>
#include <Session/State/SessionOnlineState.h>
#include <Session/State/SessionStateDefine.h>


SessionStateFactory::SessionStateFactory()
{
    RegisterProduct<SessionOnlineState>(SessionState::Online);
    RegisterProduct<SessionOfflineState>(SessionState::Offline);
}
