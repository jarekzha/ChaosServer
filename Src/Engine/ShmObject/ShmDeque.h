///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  使用共享内存的queue
//          edit by Peter_ZHA
//

#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <sstream>
#include "ShmObject/SIShmAllocator.h"
#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtr.h"

template <typename T>
class ShmDequeImpl
{
protected:
    typedef boost::interprocess::allocator<T, ShmSegmentManager> TypeAllocator;
    typedef boost::interprocess::deque<T, TypeAllocator> TypeDeque;

public:
    typedef typename TypeDeque::iterator iterator;
    typedef typename TypeDeque::size_type size_type;

public:
                        ShmDequeImpl()
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

        FindOrConstruct();
    }
                        
    virtual             ~ShmDequeImpl()       
    {
        deque->clear();
        
        std::stringstream ss;
        ss << containerID;

        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        shmAllocator.Destroy<TypeDeque>(ss.str().c_str());
        
        containerID = 0;
    }

    TypeDeque*         operator->() const
    {
        return deque;
    }

protected:
    void                FindOrConstruct()
    {
        std::stringstream ss;
        ss << containerID;
        
        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        TypeAllocator allot = TypeAllocator(shmAllocator.GetShmSegment());
        deque = shmAllocator.FindOrConstruct<TypeDeque>(ss.str().c_str())(allot);
        assert(deque != NULL);
    }

protected:
    UINT                containerID;
    TypeDeque*          deque;
};

// 基础类型使用的共享内存Deque
// 如ShmDeque<int>
template <typename T>
class ShmDeque : public ShmDequeImpl<T>
{
public:
    void                Revert(){}
};

// 自定义class使用的共享内存Deque
// 如ShmDeque<IDPtr<Player>>
template <typename T>
class ShmDeque<IDPtr<T> > : public ShmDequeImpl<IDPtr<T> >
{
public:
                        ShmDeque()
        {
            ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
            if (!shmContainerManager.NeedInit())
            {
                /*  ===重要步骤===
                * 重启恢复时注册恢复的函数指针
                * 由ShmContainerManager统一执行revert
                */
                boost::function<void()> func = boost::bind(&ShmDeque<IDPtr<T> >::Revert, this);
                shmContainerManager.RegisterRevertFunc(func);
            }
        }
    
    void                Revert()
    {
        typename ShmDequeImpl<IDPtr<T> >::TypeDeque::iterator iter;
        for (iter = this->deque->begin(); iter != this->deque->end(); ++iter)
        {
            IDPtr<T>& ptr = *iter;
            ptr.Revert();
        }
    }
};


