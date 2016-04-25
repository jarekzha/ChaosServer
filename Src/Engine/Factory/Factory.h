/*
 * Factory.h
 *
 *  Created on: 2014年7月23日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

#include <map>
#include <Object/New.h>

/*
 * 产品创建函数
 */
template <typename Product, typename AbstractProduct>
AbstractProduct* Creator()
{
    return New<Product>();
}

/*
 * Factory通用工厂类
 */
template <typename AbstractProduct>
class Factory
{
public:
    Factory(){}
    virtual             ~Factory(){}

public:
    AbstractProduct*    Create(int productType) const
    {
        typename CreatorFuncMap::const_iterator iter = creatorFuncMap.find(productType);
        if (iter == creatorFuncMap.end())
        {
            return NULL;
        }

        return (iter->second)();
    }

public:
    /*
     * 子类调用来注册产品
     */
    template <typename Product>
    void                RegisterProduct(int productType)
    {
        creatorFuncMap[productType] = Creator<Product, AbstractProduct>;
    }

private:
    typedef AbstractProduct* (*CreatorFunc)();
    typedef std::map<int, CreatorFunc> CreatorFuncMap;

    CreatorFuncMap      creatorFuncMap;     // 对象创建函数
};


