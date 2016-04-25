/*
 * RobotPlayer.h
 *
 *  Created on: 2014年7月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <Action/RobotActionList.h>
#include <Engine/Util/Util.h>
#include <Engine/XXTEA/XXTEA.h>
#include <Engine/Language/Visualizable.h>
#include <string>
#include <map>

class RobotPlayer : public Visualizable
{
public:
    RobotPlayer();
    virtual             ~RobotPlayer();

    // Visualizable
public:
    virtual std::string BriefInfo() const;

    // RobotPlayer
public:
    bool                Initialize(UINT index);

    void                SetSessionID(int sessionID);
    int                 GetSessionID();

    const std::string&  GetAccount();

    void                SetSequence(int sequence);
    int                 GetSequence();

    void                StartAction();

    bool                IsKeyInit() const;
    void                SetTeaKey(const XXTEA_TYPE* teaKey);
    const XXTEA_TYPE*   GetTeaKey();
    const XXTEA_TYPE*   GetDefaultKey();

private:
    void                ExecuteAction();

private:
    typedef boost::shared_ptr<boost::thread>    RobotPlayerThread;

private:
    RobotPlayerThread   thread;
    std::string         account;
    int                 sessionID;
    int                 sequence;
    RobotAction*        action;
    bool                keyInit;
    XXTEA_TYPE          teaKey[4];
    XXTEA_TYPE          defaultKey[4];
};

