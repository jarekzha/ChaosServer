///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  使用共享内存的list
//          edit by kcinz
//

#pragma once

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/list.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <sstream>
#include "ShmObject/SIShmAllocator.h"
#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtr.h"

template <typename T>
class ShmListImpl
{
protected:
    typedef boost::interprocess::allocator<T, ShmSegmentManager> TypeAllocator;
    typedef boost::interprocess::list<T, TypeAllocator> TypeList;

public:
    typedef typename TypeList::iterator iterator;
    typedef typename TypeList::reverse_iterator reverse_iterator;

public:
                        ShmListImpl()
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
                        
    virtual             ~ShmListImpl()
    {
        list->clear();
        
        std::stringstream ss;
        ss << containerID;

        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        shmAllocator.Destroy<TypeList>(ss.str().c_str());
        
        containerID = 0;
    }

    TypeList*           operator->() const
    {
        return list;
    }

protected:
    void                FindOrConstruct()
    {
        std::stringstream ss;
        ss << containerID;
        
        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        TypeAllocator allot = TypeAllocator(shmAllocator.GetShmSegment());
        list = shmAllocator.FindOrConstruct<TypeList>(ss.str().c_str())(allot);
        assert(list != NULL);
    }

protected:
    UINT                containerID;
    TypeList*           list;
};

// 基础类型使用的共享内存List
// 如ShmList<int>
template <typename T>
class ShmList : public ShmListImpl<T>
{
public:
    void                Revert(){} 
};

// 自定义class使用的共享内存List
// 如ShmList<IDPtr<Player>>
template <typename T>
class ShmList<IDPtr<T> > : public ShmListImpl<IDPtr<T> >
{
public:
                        ShmList()
    {
        ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
        if (!shmContainerManager.NeedInit())   // 首次使用
        {
            /*  ===重要步骤===
            * 重启恢复时注册恢复的函数指针
            * 由ShmContainerManager统一执行revert
            */
            boost::function<void()> func = boost::bind(&ShmList<IDPtr<T> >::Revert, this);
            shmContainerManager.RegisterRevertFunc(func);
        }
    }

    void                Revert()            
    {
        typename ShmListImpl<IDPtr<T> >::TypeList::iterator iter;
        for (iter = this->list->begin(); iter != this->list->end(); ++iter)
        {
            IDPtr<T>& ptr = *iter;
            ptr.Revert();
        }
    }
};



