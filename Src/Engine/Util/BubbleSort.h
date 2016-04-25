///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  冒泡排序法
//          edit by kcinz
//
//  need overload:
//          1- copy construct function
//          2- operator=
//          3- operator>
//          4- operator<
//

#pragma once

#include <vector>
#include <assert.h>

using namespace std;

template <class T>
class BubbleSort
{
public:
                        BubbleSort()
    {
        vecSort.clear();
    }
                        
    virtual             ~BubbleSort()
    {
        vecSort.clear();
    }

    void                ClearData()
    {
        vecSort.clear();
    }

    void                InitializeData(T& data)
    {
        vecSort.push_back(data);
    }

    bool               GetSort(int pos, T& data)
    {
        try
        {
            data = vecSort.at(pos);
            return true;
        }
        catch(const exception& e)
        {
            return false;
        }
    }

    int                 GetSortSize()
    {
        return (int)vecSort.size();
    }

    // 降序
    void                SortDESC()
    {
        try
        {
            for (int i = 0; i < GetSortSize(); i++)
            {
                for (int j = 1; j < (GetSortSize() - i); j++)
                {
                    if (vecSort.at(j-1) < vecSort.at(j))
                    {
                        T data = vecSort.at(j-1);
                        vecSort.at(j-1) = vecSort.at(j);
                        vecSort.at(j) = data;
                    }
                }
            }
        }
        catch(const exception& e)
        {
            assert(false);
            return;
        }
    }

    // 升序
    void                SortASC()
    {
        try
        {
            for (int i = 0; i < GetSortSize(); i++)
            {
                for (int j = 1; j < (GetSortSize() - i); j++)
                {
                    if (vecSort.at(j-1) > vecSort.at(j))
                    {
                        T data = vecSort.at(j-1);
                        vecSort.at(j-1) = vecSort.at(j);
                        vecSort.at(j) = data;
                    }
                }
            }
        }
        catch(const exception& e)
        {
            assert(false);
            return;
        }
    }

private:
    typedef vector<T>  SortVector;

    SortVector          vecSort;
};

