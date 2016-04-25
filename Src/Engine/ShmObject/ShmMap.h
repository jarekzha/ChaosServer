// *****************************************
// 使用共享内存的ShmMap
// by: Peter_ZHA
#pragma once
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/pair.hpp>
#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <sstream>
#include "ShmObject/SIShmAllocator.h"
#include "ShmObject/ShmContainerManager.h"
#include "ShmObject/IDPtr.h"

// boost::container::container_detail

template <typename KeyType, typename Type>
class ShmMapImpl
{
protected:
    typedef std::pair<const KeyType, Type> PairType;
    typedef boost::interprocess::allocator<PairType, ShmSegmentManager> TypeAllocator;
    typedef boost::interprocess::map<KeyType, Type, std::less<KeyType>, TypeAllocator> TypeMap;

public:
    typedef typename TypeMap::iterator iterator;
    typedef typename TypeMap::size_type size_type;
    typedef typename TypeMap::value_type value_type;

public:
                        ShmMapImpl()
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
                        
    virtual             ~ShmMapImpl()       
    {
        map->clear();
        
        std::stringstream ss;
        ss << containerID;

        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        shmAllocator.Destroy<TypeMap>(ss.str().c_str());
        
        containerID = 0;
    }

    TypeMap*            operator->()
    {
        return map;
    }

    Type&               operator[](KeyType key)
    {
        return map->operator[](key);
    }

protected:
    void                _FindOrConstruct()
    {
        std::stringstream ss;
        ss << containerID;
        
        SIShmAllocator& shmAllocator = SIShmAllocator::Instance();
        TypeAllocator allot = TypeAllocator(shmAllocator.GetShmSegment());
        map = shmAllocator.FindOrConstruct<TypeMap>(ss.str().c_str())(std::less<KeyType>() , allot);
        assert(map != NULL);
    }

protected:
    UINT                containerID;
    TypeMap*            map;
};

// 基础类型使用的共享内存Map
// 如ShmMap<int, int>
template <typename KeyType, typename Type>
class ShmMap : public ShmMapImpl<KeyType, Type>
{
public:
    void                Revert(){}
};

// 自定义class使用的共享内存Vector
// 如ShmMap<int, IDPtr<Player>>
template <typename KeyType, typename Type>
class ShmMap<KeyType, IDPtr<Type> > : public ShmMapImpl<KeyType, IDPtr<Type> >
{
public:
                        ShmMap()
        {
            ShmContainerManager& shmContainerManager = ShmContainerManager::Instance();
            if (!shmContainerManager.NeedInit())
            {
                /*  ===重要步骤===
                * 重启恢复时注册恢复的函数指针
                * 由ShmContainerManager统一执行revert
                */
                boost::function<void()> func = boost::bind(&ShmMap<KeyType, IDPtr<Type> >::Revert, this);
                shmContainerManager.RegisterRevertFunc(func);
            }
        }
    
    void                Revert()
    {
        typename ShmMapImpl<KeyType, IDPtr<Type> >::iterator iter;
        for (iter = this->map->begin(); iter != this->map->end(); ++iter)
        {
            IDPtr<Type>& ptr = iter->second;
            ptr.Revert();
        }
    }
};

