/*
 * Cloneable.h
 *
 *  Created on: 2014年7月30日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

template <typename T>
class Cloneable
{
public:
    virtual             ~Cloneable(){}

    virtual T*          Clone() const = 0;
};

