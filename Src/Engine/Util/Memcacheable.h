/*
 * Memcacheable.h
 *
 *  Created on: 2015年7月15日
 *  Author: jarek
 *  Brief: 
 */

#pragma once

class Memcacheable
{
public:
    Memcacheable(): m_firstSave(true){}
    virtual             ~Memcacheable(){}

    // 从cache中加载
    virtual bool        LoadFromCache() = 0;

    // 保存到cache中
    virtual std::string SaveToCache(bool saveRelatedObj) = 0;

protected:
    bool                IsFirstSave(){return m_firstSave;}

    void                MarkSave(){m_firstSave = false;}

private:
    bool                m_firstSave;
};


