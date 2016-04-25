#pragma once

#include <boost/interprocess/managed_xsi_shared_memory.hpp>
#include <string>
#include "Util/TMTSingleton.h"

typedef boost::interprocess::managed_xsi_shared_memory  ShmManager;
typedef ShmManager::segment_manager                     ShmSegmentManager;
typedef boost::interprocess::interprocess_exception     ShmException;
typedef boost::interprocess::xsi_key                    Shmkey;

class SIShmAllocator : public TMTSingleton<SIShmAllocator>
{
public:
                        SIShmAllocator();
                        ~SIShmAllocator();

    bool                Initialize(int size = 209715200);

    ShmSegmentManager* GetShmSegment();

    template <typename T>
    typename ShmSegmentManager::construct_proxy<T>::type FindOrConstruct(const char* name);

    template <typename T>
    bool Destroy(const char* name);

private:
    ShmManager*         shmManager;

    int                 shmID;
};

template <typename T>
typename ShmSegmentManager::construct_proxy<T>::type SIShmAllocator::FindOrConstruct(const char* name)
{
    return shmManager->find_or_construct<T>(name);
}

template <typename T>
bool SIShmAllocator::Destroy(const char* name)
{
    return shmManager->destroy<T>(name);
}



