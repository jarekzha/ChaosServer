/*
 * TResHolder.h
 *
 *  Created on: 2015年11月27日
 *  Author: seekerwang
 *  Brief: 
 */

#pragma once

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "tresloader.h"
#include <string>

class IGameDataMgr
{
public:
    IGameDataMgr() {}
    virtual             ~IGameDataMgr() {}

public:
    virtual bool        Load() = 0;         //资源加载
};

// TKey目前只能是2,4,8字节int
template <typename TRES, typename TKEY = int>
class TGameDataMgr : public IGameDataMgr
{
public:
    TGameDataMgr(const std::string& dirname)
        : m_dirName(dirname)
        , m_pszBuff(NULL)
        , m_iBuff(0)
        , m_iUint(0)
        , m_iResNum(0)
    {

    }
    virtual             ~TGameDataMgr()
    {
        if (NULL != m_pszBuff)
        {
            trl_unload( &m_pszBuff );
        }
    }

    //IResDataMgr
public:
    virtual bool        Load();

public:
    virtual bool        PreProcess() { return true; }
    virtual bool        Reload();

    bool                Unload()
    {
        if (NULL != m_pszBuff)
        {
            trl_unload( &m_pszBuff );
        }
        m_pszBuff = NULL;
        m_iBuff = 0;
        m_iUint = 0;
        m_iResNum = 0;
        return true;
    }

    bool                Load(const char* pszResFilePath);
    TRES*               Find( TKEY nKey ) const;

    int                 GetResNum() const { return m_iResNum; }
    char*               GetResBuf( ) { return m_pszBuff; }
    int                 GetResBufSize() { return m_iBuff; }
    int                 GetUnitSize() { return m_iUint; }

    TRES*               GetResByPos( int iPos );

    const TRES*         GetResByPos(int iPos) const;

private:
    std::string         m_dirName;    // 资源路径
    char*               m_pszBuff;    // 保存数据的数据区指针的指针
    int                 m_iBuff;      // 数据区的字节数
    int                 m_iUint;      // 单个资源信息结构体的在缓冲区中的存储空间大小
    int                 m_iResNum;    // 缓冲区中保存的资源信息结构体的个数
};

template <typename TRES, typename TKEY>
bool TGameDataMgr<TRES, TKEY>::Load()
{
    return Load(m_dirName.c_str());
}

template <typename TRES, typename TKEY>
bool TGameDataMgr<TRES, TKEY>::Reload()
{
    Unload();
    return Load(m_dirName.c_str());
}

template <typename TRES, typename TKEY>
bool TGameDataMgr<TRES, TKEY>::Load(const char* pszResFilePath)
{
    if( NULL == pszResFilePath )
    {
        assert( false );
        return false;
    }

    int iRet = trl_load( &m_pszBuff, &m_iBuff, &m_iUint, pszResFilePath, NULL, 0 );
    if( iRet <= 0 )
    {
        return false;
    }

    m_iResNum = iRet;

    if(!PreProcess())
    {
        return false;
    }

    return true;
}

template <typename TRES, typename TKEY>
TRES* TGameDataMgr<TRES, TKEY>::Find( TKEY nKey ) const
{
    if( NULL == m_pszBuff )
    {
        return NULL;
    }

    char* pszRes = NULL;
    switch( sizeof(TKEY) )
    {
        case 2:
        {
            pszRes = trl_find_n( m_pszBuff, m_iResNum, m_iUint, nKey );
        } break;
        case 4:
        {
            pszRes = trl_find( m_pszBuff, m_iResNum, m_iUint, nKey );
        } break;
        case 8:
        {
            pszRes = trl_find_ll( m_pszBuff, m_iResNum, m_iUint, nKey );
        } break;
        default : break;
    }

    return (TRES *)pszRes;
}


template <typename TRES, typename TKEY>
TRES*  TGameDataMgr<TRES, TKEY>::GetResByPos( int iPos )
{
    if( iPos < 0 || iPos >= m_iResNum )
    {
        return NULL;
    }

    return (TRES*)( m_pszBuff + iPos * sizeof(TRES) );
}

template <typename TRES, typename TKEY>
const TRES* TGameDataMgr<TRES, TKEY>::GetResByPos(int iPos) const
{
    if (iPos < 0 || iPos >= m_iResNum)
    {
        return NULL;
    }

    return (TRES*)(m_pszBuff + iPos * sizeof(TRES));
}
