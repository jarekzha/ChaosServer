/*
 * GracefullyStopable.h
 *
 *  Created on: 2014年10月29日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

/**
 * 可以被优雅停止的组件或进程
 */
class GracefullyStopable
{
public:
    virtual             ~GracefullyStopable(){}

    // 开始优雅停止
    virtual void        StartGracefullyStop() = 0;

    // 检测优雅停止是否结束
    virtual bool        CheckGracefullyStopFinished() = 0;
};


