/*
 * Sendable.h
 *
 *  Created on: 2014年7月19日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

/*
 * 可以被发送的东西
 */
class Sendable
{
public:
    virtual ~Sendable(){}

    // 发送
    virtual void        Send() = 0;
};



