// FakeRandom.h: interface for the FakeRandom class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "TMTSingleton.h"
#include "Util.h"


class FakeRandom : public TMTSingleton<FakeRandom>
{
public:
                        FakeRandom()
    {
        seed = 0;
    }

    //--------------------------------------------------------------------
    // 设置随机数种子
    void                SetSeed(UINT uiSeed)
    {
        seed = uiSeed;
    }

    //--------------------------------------------------------------------
    // 生成一个随机数
    UINT                Random()
    {
        UINT next = seed;
        UINT result;

        next *= 1103515245;
        next += 12345;
        result = (UINT) (next / 65536) % 2048;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (UINT) (next / 65536) % 1024;

        next *= 1103515245;
        next += 12345;
        result <<= 10;
        result ^= (UINT) (next / 65536) % 1024;

        seed = next;

        return result;
    }

    //--------------------------------------------------------------------
    // 生成一个[0, unRange - 1]之间的随机数
    UINT                Random(UINT uiRange)
    {
        if (uiRange == 0)
            return 0;

        return Random() % uiRange;
    }

    // 生成一个[unMinRange, unMaxRange - 1]之间的随机数
    UINT                Random(UINT uiMinRange, UINT uiMaxRange)
    {
        if (uiMinRange == uiMaxRange)
        {
            return uiMaxRange;
        }

        if (uiMaxRange < uiMinRange)
        {
            return 0;
        }

        return Random(uiMaxRange - uiMinRange) + uiMinRange;
    }

    //生成uiNum个小于uiRange的不重复随机数,返回值放入puiList中
    UINT                Random(UINT uiRange, UINT uiNum, UINT *puiList)
    {
        if(NULL == puiList)
        {
            return 0;
        }

        UINT a[uiRange];
        for(UINT i = 0; i < uiRange; ++i)
        {
            a[i] = i;
        }

        for(UINT i = 0; i < uiNum; ++i)
        {
            UINT w = Random(uiRange - i) + i;
            UINT t = a[i];
            a[i] = a[w];
            a[w] = t;
        }

        for(UINT i = 0; i < uiNum; ++i)
        {
            puiList[i] = a[i];
        }

        return 1;
    }

    // 判断是否几率达成，wRate是以万分比为单位的几率值
    bool                HitRate(WORD wRate)
    {
        if (wRate >= 10000)
        {
            return true;
        }
        else if (wRate == 0)
        {
            return false;
        }

        return (Random(10000) < wRate);
    }

private:
    UINT                seed;
};

