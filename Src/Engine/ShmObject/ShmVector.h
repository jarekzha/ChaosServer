///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  使用共享内存的vector
//          edit by kcinz
//

#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <sstream>
#include "ShmObject/SIShmAllocator.h"
#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtr.h"

template <typename T>
class ShmVectorImpl
{
protected:
    typedef boost::interprocess::allocator<T, ShmSegmentManager> TypeAllocator;
    typedef boost::interprocess::vector<T, TypeAllocator> TypeVector;

public:
    typedef typename TypeVector::iterator iterator;
    typedef typename TypeVector::size_type size_type;
    typedef typename TypeVector::reverse_iterator reverse_iterator;

public:
                        ShmVectorImpl()
    {
        ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
        if (shmContainerManager.NeedInit())   // 首次使用
        {
            containerID = shmContainerManager.GetNewContainerID();
        }
        else
        {
            shmContainerManager.RecoverID(containerID);
        }

        _FindOrConstruct();
    }
                        
    virtual             ~ShmVectorImpl()       
    {
        vector->clear();
        
        std::stringstream ss;
        ss << containerID;

        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        shmAllocator.Destroy<TypeVector>(ss.str().c_str());
        
        containerID = 0;
    }

    TypeVector*         operator->() const
    {
        return vector;
    }

    T&                  operator[](size_type index)
    {
        return vector->operator[](index);
    }

protected:
    void                _FindOrConstruct()
    {
        std::stringstream ss;
        ss << containerID;
        
        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        TypeAllocator allot = TypeAllocator(shmAllocator.GetShmSegment());
        vector = shmAllocator.FindOrConstruct<TypeVector>(ss.str().c_str())(allot);
        assert(vector != NULL);
    }

protected:
    UINT                containerID;
    TypeVector*         vector;
};

// 基础类型使用的共享内存Vector
// 如ShmVector<int>
template <typename T>
class ShmVector : public ShmVectorImpl<T>
{
public:
    void                Revert(){}
};

// 自定义class使用的共享内存Vector
// 如ShmVector<IDPtr<Player>>
template <typename T>
class ShmVector<IDPtr<T> > : public ShmVectorImpl<IDPtr<T> >
{
public:
                        ShmVector()
        {
            ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
            if (!shmContainerManager.NeedInit())
            {
                /*  ===重要步骤===
                * 重启恢复时注册恢复的函数指针
                * 由ShmContainerManager统一执行revert
                */
                boost::function<void()> func = boost::bind(&ShmVector<IDPtr<T> >::Revert, this);
                shmContainerManager.RegisterRevertFunc(func);
            }
        }
    
    void                Revert()
    {
        typename ShmVectorImpl<IDPtr<T> >::TypeVector::iterator iter;
        for (iter = this->vector->begin(); iter != this->vector->end(); ++iter)
        {
            IDPtr<T>& ptr = *iter;
            ptr.Revert();
        }
    }
};


