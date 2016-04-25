/*
 * SignalHandler.h
 *
 *  Created on: 2014年8月13日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

// 信号：输出IPC信息
#define SIG_IPC_HANDLE_PRINT        (SIGRTMIN)

// 信号：输出对象计数器信息
#define SIG_OBJECT_COUNTER_PRINT    (SIGRTMIN+1)

// 信号：重新加载配置
#define SIG_RELOAD_CONFIG           (SIGRTMIN+2)

void SigIPCHandlePrint(int);

void SigObjectCounterPrint(int);

void InitSignalHandler();

