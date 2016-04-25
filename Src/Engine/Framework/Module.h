/*
 * Module.h
 *
 *  Created on: 2014年8月7日
 *  Author: jarek
 *  Brief: 
 */

#pragma once
#include <Framework/GracefullyStopable.h>

class Module
{
public:
    virtual             ~Module(){}

    virtual bool        Initialize() = 0;
};


class LogicModule : public Module , public GracefullyStopable
{
public:
    // 开始优雅停止
    virtual void        StartGracefullyStop(){}

    // 检测优雅停止是否结束
    virtual bool        CheckGracefullyStopFinished(){return true;}
};


class ConfigModule : public Module
{
    // NULL
};
